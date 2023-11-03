#include "carbuncle/core.h"
#include "carbuncle/point.h"
#include "carbuncle/color.h"
#include "carbuncle/tilemap.h"
#include "carbuncle/filesystem.h"

#include "raylib.h"

#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/array.h>
#include <mruby/variable.h>
#include <mruby/hash.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tmx_xml.h"

static mrb_state *tilemap_current_mrb;

static void *
carbuncle_texture_loader(const char *path)
{
  Texture2D texture = LoadCarbuncleTexture(tilemap_current_mrb, path);
  Texture2D *value = malloc(sizeof *value);
  memcpy(value, &texture, sizeof *value);
  return value;
}

static void
carbuncle_texture_free(void *ptr)
{
  UnloadTexture(*((Texture2D*)ptr));
  free(ptr);
}

static xmlTextReaderPtr
carbuncle_xml_reader_new(const char *path, const char *encoding, int options)
{
  size_t size;
  char *buffer = mrb_carbuncle_load_file(tilemap_current_mrb, path, &size);
  xmlTextReaderPtr reader = xmlReaderForMemory(buffer, size, path, encoding, options);
  mrb_carbuncle_add_buffer(reader, buffer);
  return reader;
}

static void
carbuncle_xml_free(xmlTextReaderPtr reader)
{
  xmlFreeTextReader(reader);
  char *buffer = mrb_carbuncle_get_buffer(reader);
  mrb_free(tilemap_current_mrb, buffer);
  mrb_carbuncle_remove_buffer(reader);
}

static void
mrb_tilemap_free(mrb_state *mrb, void *ptr)
{
  if (ptr)
  {
    tmx_map *map = ptr;
    for (size_t i = 0; i < map->tilecount; ++i)
    {
      tmx_tile *tile = map->tiles[i];
      if (tile && tile->animation_len)
      {
        mrb_free(mrb, tile->user_data.pointer);
      }
    }
    tmx_map_free(map);
  }
}

static void
mrb_no_free(mrb_state *mrb, void *ptr)
{
}

static const char *
get_layer_class(tmx_layer *layer)
{
  switch (layer->type)
  {
    case L_GROUP: { return "LayerGroup"; }
    case L_OBJGR: { return "ObjectLayer"; }
    case L_IMAGE: { return "ImageLayer"; }
    case L_LAYER: { return "TileLayer"; }
    default: { return "Layer"; }
  }
}

static const struct mrb_data_type tilemap_data_type = {
  "Carbuncle::Tilemap", mrb_tilemap_free
};

static const struct mrb_data_type layer_data_type = {
  "Carbuncle::Tilemap::Layer", mrb_no_free
};

static const struct mrb_data_type tile_data_type = {
  "Carbuncle::Tilemap::Tile", mrb_no_free
};

static mrb_value
property_value(mrb_state *mrb, tmx_property *property)
{
  switch (property->type)
  {
    case PT_BOOL: { return mrb_bool_value(property->value.boolean); }
    case PT_COLOR:
    {
      Color color = GetColor(property->value.color);
      return mrb_obj_freeze(mrb, mrb_carbuncle_color_new(mrb, color.r, color.g, color.b, color.a));
    }
    case PT_FLOAT: { return mrb_float_value(mrb, property->value.decimal); }
    case PT_INT: { return mrb_fixnum_value(property->value.integer); }
    case PT_NONE: { return mrb_true_value(); }
    case PT_FILE: { return mrb_str_new_cstr(mrb, property->value.file); }
    case PT_STRING: { return mrb_str_new_cstr(mrb, property->value.string); }
    default: { break; }
  }
  return mrb_true_value();
}

static void
set_property(tmx_property *property, void *userdata)
{
  struct mrb_PropertyIterator *iterator = userdata;
  mrb_state *mrb = iterator->mrb;
  mrb_value value = property_value(mrb, property);
  mrb_hash_set(mrb, iterator->hash, mrb_str_new_cstr(mrb, property->name), value);
}

static void
setup_properties(mrb_state *mrb, mrb_value self, tmx_properties *properties)
{
  mrb_value hash = mrb_hash_new(mrb);
  struct mrb_PropertyIterator iterator = (struct mrb_PropertyIterator){ mrb, hash };
  tmx_property_foreach(properties, set_property, &iterator);
  mrb_iv_set(mrb, self, PROPERTIES_SYMBOL, mrb_obj_freeze(mrb, hash));
}

static void
setup_layers(mrb_state *mrb, tmx_map *map, mrb_value self)
{
  struct RClass *tilemap = mrb_carbuncle_class_get(mrb, "Tilemap");
  tmx_layer *layer;
  layer = map->ly_head;
  while (layer)
  {
    struct RClass *layer_class = mrb_class_get_under(mrb, tilemap, get_layer_class(layer));
    mrb_value layer_value = mrb_obj_new(mrb, layer_class, 1, &self);
    layer->opacity = layer->opacity * 255;
    DATA_PTR(layer_value) = layer;
    DATA_TYPE(layer_value) = &layer_data_type;
    setup_properties(mrb, layer_value, layer->properties);
    layer = layer->next;
  }
}

static void
setup_tiles(mrb_state *mrb, tmx_map *map, mrb_value self)
{
  struct RClass *map_class  = mrb_carbuncle_class_get(mrb, "Tilemap");
  struct RClass *tile_class = mrb_class_get_under(mrb, map_class, "Tile");
  mrb_value tiles = mrb_ary_new_capa(mrb, map->tilecount);
  for (size_t i = 0; i < map->tilecount; ++i)
  {
    tmx_tile *tile = map->tiles[i];
    if (tile)
    {
      mrb_value tile_obj = mrb_obj_new(mrb, tile_class, 1, &self);
      DATA_TYPE(tile_obj) = &tile_data_type;
      DATA_PTR(tile_obj) = tile;
      mrb_ary_set(mrb, tiles, i, tile_obj);
      setup_properties(mrb, tile_obj, tile->properties);
      if (tile->animation_len)
      {
        struct mrb_TileData *data = mrb_malloc(mrb, sizeof *data);
        data->frame = 0;
        data->frame_time = 0;
        tile->user_data.pointer = data;
      }
    }
  }
  mrb_iv_set(mrb, self, TILES_SYMBOL, mrb_obj_freeze(mrb, tiles));
}

static mrb_value
mrb_tilemap_initialize(mrb_state *mrb, mrb_value self)
{
  int arena;
  const char *name;
  tmx_map *map;
  mrb_get_args(mrb, "z", &name);
  tilemap_current_mrb = mrb;
  map = tmx_load(name);
  if (!map)
  {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "Failed to load map '%s' (%s)", name, tmx_strerr());
  }
  DATA_PTR(self) = map;
  DATA_TYPE(self) = &tilemap_data_type;
  arena = mrb_gc_arena_save(mrb);
  mrb_value layers = mrb_ary_new(mrb);
  mrb_iv_set(mrb, self, LAYERS_SYMBOL, layers);
  mrb_iv_set(mrb, self, OFFSET_SYMBOL, mrb_carbuncle_point_new(mrb, 0, 0));
  setup_layers(mrb, map, self);
  setup_tiles(mrb, map, self);
  setup_properties(mrb, self, map->properties);
  mrb_gc_arena_restore(mrb, arena);
  return self;
}

static mrb_value
mrb_tilemap_get_layers(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, LAYERS_SYMBOL);
}

static mrb_value
mrb_tilemap_get_offset(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, OFFSET_SYMBOL);
}

static mrb_value
mrb_tilemap_get_width(mrb_state *mrb, mrb_value self)
{
  tmx_map *map = mrb_carbuncle_get_tilemap(mrb, self);
  return mrb_int_value(mrb, map->width);
}

static mrb_value
mrb_tilemap_get_height(mrb_state *mrb, mrb_value self)
{
  tmx_map *map = mrb_carbuncle_get_tilemap(mrb, self);
  return mrb_int_value(mrb, map->height);
}

static mrb_value
mrb_tilemap_get_tile_width(mrb_state *mrb, mrb_value self)
{
  tmx_map *map = mrb_carbuncle_get_tilemap(mrb, self);
  return mrb_int_value(mrb, map->tile_width);
}

static mrb_value
mrb_tilemap_get_tile_height(mrb_state *mrb, mrb_value self)
{
  tmx_map *map = mrb_carbuncle_get_tilemap(mrb, self);
  return mrb_int_value(mrb, map->tile_height);
}

static mrb_value
mrb_tilemap_set_offset(mrb_state *mrb, mrb_value self)
{
  mrb_value value;
  mrb_get_args(mrb, "o", &value);
  mrb_carbuncle_get_point(mrb, value);
  mrb_iv_set(mrb, self, OFFSET_SYMBOL, value);
  return value;
}

static mrb_value
mrb_tilemap_disposedQ(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(!DATA_PTR(self));
}

static mrb_value
mrb_tilemap_dispose(mrb_state *mrb, mrb_value self)
{
  mrb_carbuncle_get_tilemap(mrb, self);
  mrb_tilemap_free(mrb, DATA_PTR(self));
  DATA_PTR(self) = NULL;
  mrb_iv_set(mrb, self, LAYERS_SYMBOL, mrb_ary_new(mrb));
  return self;
}

static mrb_value
mrb_tilemap_update(mrb_state *mrb, mrb_value self)
{
  size_t len;
  mrb_value dtv, layers;
  mrb_float dt;
  tmx_map *map = mrb_carbuncle_get_tilemap(mrb, self);
  mrb_get_args(mrb, "f", &dt);
  dtv = mrb_float_value(mrb, dt);
  layers = mrb_iv_get(mrb, self, LAYERS_SYMBOL);
  len = RARRAY_LEN(layers);
  for (size_t i = 0; i < map->tilecount; ++i)
  {
    tmx_tile *tile = map->tiles[i];
    if (tile && tile->animation_len)
    {
      struct mrb_TileData *data = tile->user_data.pointer;
      tmx_anim_frame animation = tile->animation[data->frame];
      data->frame_time += dt * 1000;
      while (data->frame_time >= animation.duration)
      {
        data->frame_time -= animation.duration;
        data->frame = (data->frame + 1) % tile->animation_len;
        animation = tile->animation[data->frame];
      }
    }
  }
  for (size_t i = 0; i < len; ++i)
  {
    mrb_funcall(mrb, mrb_ary_entry(layers, i), "update", 1, dtv);
  }
  return self;
}

static mrb_value
mrb_tilemap_draw(mrb_state *mrb, mrb_value self)
{
  size_t len;
  mrb_value layers;
  tmx_map *map = mrb_carbuncle_get_tilemap(mrb, self);
  layers = mrb_iv_get(mrb, self, LAYERS_SYMBOL);
  len = RARRAY_LEN(layers);
  ClearBackground(GetColor(map->backgroundcolor));
  for (size_t i = 0; i < len; ++i)
  {
    mrb_funcall(mrb, mrb_ary_entry(layers, i), "draw", 0);
  }
  return self;
}

static mrb_value
mrb_tilemap_get_properties(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, PROPERTIES_SYMBOL);
}

static mrb_value
mrb_tilemap_get_tiles(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, TILES_SYMBOL);
}

struct RClass *
mrb_init_carbuncle_tilemap(mrb_state *mrb)
{
  struct RClass *carbuncle = mrb_carbuncle_get(mrb);
  struct RClass *tilemap = mrb_define_class_under(mrb, carbuncle, "Tilemap", mrb->object_class);
  MRB_SET_INSTANCE_TT(tilemap, MRB_TT_DATA);

  tmx_img_load_func = carbuncle_texture_loader;
  tmx_img_free_func = carbuncle_texture_free;
  tmx_xml_reader_load_func = carbuncle_xml_reader_new;
  tmx_xml_reader_free_func = carbuncle_xml_free;

  mrb_define_method(mrb, tilemap, "initialize", mrb_tilemap_initialize, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, tilemap, "disposed?", mrb_tilemap_disposedQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, tilemap, "dispose", mrb_tilemap_dispose, MRB_ARGS_NONE());

  mrb_define_method(mrb, tilemap, "layers", mrb_tilemap_get_layers, MRB_ARGS_NONE());
  mrb_define_method(mrb, tilemap, "tiles", mrb_tilemap_get_tiles, MRB_ARGS_NONE());
  mrb_define_method(mrb, tilemap, "properties", mrb_tilemap_get_properties, MRB_ARGS_NONE());
  mrb_define_method(mrb, tilemap, "offset", mrb_tilemap_get_offset, MRB_ARGS_NONE());
  mrb_define_method(mrb, tilemap, "width", mrb_tilemap_get_width, MRB_ARGS_NONE());
  mrb_define_method(mrb, tilemap, "height", mrb_tilemap_get_height, MRB_ARGS_NONE());
  mrb_define_method(mrb, tilemap, "tile_width", mrb_tilemap_get_tile_width, MRB_ARGS_NONE());
  mrb_define_method(mrb, tilemap, "tile_height", mrb_tilemap_get_tile_height, MRB_ARGS_NONE());

  mrb_define_method(mrb, tilemap, "offset=", mrb_tilemap_set_offset, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, tilemap, "update", mrb_tilemap_update, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, tilemap, "draw", mrb_tilemap_draw, MRB_ARGS_NONE());

  return tilemap;
}

tmx_map *
mrb_carbuncle_get_tilemap(mrb_state *mrb, mrb_value obj)
{
  return DATA_GET_DISPOSABLE_PTR(mrb, obj, &tilemap_data_type, tmx_map);
}

mrb_bool
mrb_carbuncle_tilemap_p(mrb_value obj)
{
  return mrb_data_p(obj) && (DATA_TYPE(obj) == &tilemap_data_type);
}
