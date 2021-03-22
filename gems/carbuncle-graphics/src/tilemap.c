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

#define LAYERS_SYMBOL     mrb_intern_cstr(mrb, "#layers")
#define MAP_SYMBOL        mrb_intern_cstr(mrb, "#map")
#define OFFSET_SYMBOL     mrb_intern_cstr(mrb, "#offset")
#define PROPERTIES_SYMBOL mrb_intern_cstr(mrb, "#properties")
#define TILES_SYMBOL      mrb_intern_cstr(mrb, "#tiles")

static mrb_state *tilemap_current_mrb;
struct mrb_TileData
{
  mrb_int frame;
  float   frame_time;
};

struct mrb_PropertyIterator
{
  mrb_state *mrb;
  mrb_value hash;
};

static inline mrb_float
tilemap_clamp(mrb_float value, mrb_float min, mrb_float max)
{
  if (value < min) { return min; }
  if (value > max) { return max; }
  return value;
}

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

static void
draw_layer(Vector2 *offset, tmx_map *map, tmx_layer *layer);

static void
draw_all_layers(Vector2 *offset, tmx_map *map, tmx_layer *layer)
{
  while (layer)
  {
    draw_layer(offset, map, layer);
    layer = layer->next;
  }
}

static void
draw_image_layer(Vector2 *offset, tmx_layer *layer, tmx_image *image)
{
  Texture2D *texture = (Texture2D*)image->resource_image;
  DrawTexture(*texture, layer->offsetx - offset->x, layer->offsety - offset->y, WHITE);
}

static void
draw_tile(Vector2 *offset, void *image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
               unsigned int dx, unsigned int dy, float opacity, unsigned int flags)
{
  Rectangle src = (Rectangle) {sx, sy, sw, sh};
  Vector2 pos = (Vector2) {dx - offset->x, dy - offset->y};
  Texture2D *img = image;
  Color color = (Color) {opacity, opacity, opacity, opacity};
  DrawTextureRec(*img, src, pos, color);
}

static void
draw_tile_layer(Vector2 *offset, tmx_map *map, tmx_layer *layer)
{
  tmx_tile *tile;
  unsigned long i, j;
  unsigned int gid, x, y, w, h, flags;
  float op;
  tmx_tileset *ts;
  tmx_image *im;
  void* image;
  op = layer->opacity;
  for (i=0; i<map->height; i++)
  {
    for (j=0; j<map->width; j++)
    {
      gid = layer->content.gids[(i*map->width)+j];
      tile = map->tiles[gid];
      if (tile->animation_len)
      {
        struct mrb_TileData *data = tile->user_data.pointer;
        tmx_anim_frame animation = tile->animation[data->frame];
        tile = map->tiles[animation.tile_id + 1];
      }
      if (tile)
      {
        ts = tile->tileset;
        im = tile->image;
        x  = tile->ul_x;
        y  = tile->ul_y;
        w  = ts->tile_width;
        h  = ts->tile_height;
        image = im ? im->resource_image : ts->image->resource_image;
        flags = layer->content.gids[(i*map->width)+j];
        draw_tile(offset, image, x, y, w, h, j * ts->tile_width, i * ts->tile_height, op, flags);
      }
    }
  }
}

static void
draw_polyline(Vector2 *offset, Color *color, double **points, double x, double y, size_t size)
{
  for (size_t i = 1; i < size; i++) {
    DrawLine(
      x + points[i-1][0] - offset->x,
      y + points[i-1][1] - offset->y,
      x + points[i][0],
      y + points[i][1],
      *color
    );
  }
}

void draw_polygon(Vector2 *offset, Color *color, double **points, double x, double y, size_t size)
{
  draw_polyline(offset, color, points, x, y, size);
  if (size > 2)
  {
    DrawLine(
      x + points[0][0] - offset->x,
      y + points[0][1] - offset->y,
      x + points[size - 1][0],
      y + points[size - 1][1],
    *color
    );
  }
}

static void
draw_objects(Vector2 *offset, tmx_object_group *objgr) {
  Rectangle rect;
  Color color = GetColor(objgr->color);
  tmx_object *head = objgr->head;

  while (head)
  {
    if (head->visible)
    {
      switch (head->obj_type)
      {
        case OT_SQUARE:
        {
          rect.x = head->x - offset->x;
          rect.y = head->y - offset->y;
          rect.width = head->width;
          rect.height = head->height;
          DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, color);
          break;
        }
        case OT_POLYGON:
        {
          draw_polygon(offset, &color, head->content.shape->points, head->x, head->y, head->content.shape->points_len);
          break;
        }
        case OT_POLYLINE:
        {
          draw_polyline(offset, &color, head->content.shape->points, head->x, head->y, head->content.shape->points_len);
          break;
        }
        case OT_ELLIPSE:
        {
          // TODO: Add ellipse
          break;
        }
        default: { break; }
      }
    }
    head = head->next;
  }
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

static void
draw_layer(Vector2 *offset, tmx_map *map, tmx_layer *layer)
{
  if (layer->visible)
  {
    Vector2 scale = { 1, 1 };
    tmx_property *sx = tmx_get_property(layer->properties, "Carbuncle Offset Scale X");
    tmx_property *sy = tmx_get_property(layer->properties, "Carbuncle Offset Scale Y");
    if (sx && sx->type == PT_FLOAT) { scale.x = sx->value.decimal; }
    if (sy && sy->type == PT_FLOAT) { scale.y = sy->value.decimal; }
    Vector2 layer_offset = { offset->x * scale.x, offset->y * scale.y };
    switch (layer->type)
    {
      case L_GROUP:
      {
        draw_all_layers(&layer_offset, map, layer->content.group_head);
        break;
      }
      case L_OBJGR:
      {
        draw_objects(&layer_offset, layer->content.objgr);
        break;
      }
      case L_IMAGE:
      {
        draw_image_layer(&layer_offset, layer, layer->content.image);
        break;
      }
      case L_LAYER:
      {
        draw_tile_layer(&layer_offset, map, layer);
        break;
      }
      default: { break; }
    }
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
  struct RClass *tilemap     = mrb_carbuncle_class_get(mrb, "Tilemap");
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
  return mrb_fixnum_value(map->width);
}

static mrb_value
mrb_tilemap_get_height(mrb_state *mrb, mrb_value self)
{
  tmx_map *map = mrb_carbuncle_get_tilemap(mrb, self);
  return mrb_fixnum_value(map->height);
}

static mrb_value
mrb_tilemap_get_tile_width(mrb_state *mrb, mrb_value self)
{
  tmx_map *map = mrb_carbuncle_get_tilemap(mrb, self);
  return mrb_fixnum_value(map->tile_width);
}

static mrb_value
mrb_tilemap_get_tile_height(mrb_state *mrb, mrb_value self)
{
  tmx_map *map = mrb_carbuncle_get_tilemap(mrb, self);
  return mrb_fixnum_value(map->tile_height);
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
mrb_layer_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value map;
  mrb_get_args(mrb, "o", &map);
  mrb_carbuncle_get_tilemap(mrb, map);
  mrb_iv_set(mrb, self, MAP_SYMBOL, map);
  mrb_ary_push(mrb, mrb_iv_get(mrb, map, LAYERS_SYMBOL), self);
  return self;
}

static mrb_value
mrb_layer_get_map(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, MAP_SYMBOL);
}

static mrb_value
mrb_layer_get_visible(mrb_state *mrb, mrb_value self)
{
  tmx_layer *layer = DATA_PTR(self);
  return mrb_bool_value( layer->visible );
}

static mrb_value
mrb_layer_get_opacity(mrb_state *mrb, mrb_value self)
{
  tmx_layer *layer = DATA_PTR(self);
  return mrb_float_value(mrb, layer->opacity);
}

static mrb_value
mrb_layer_set_visible(mrb_state *mrb, mrb_value self)
{
  mrb_bool value;
  tmx_layer *layer = DATA_PTR(self);
  mrb_get_args(mrb, "b", &value);
  layer->visible = value;
  return mrb_bool_value( layer->visible );
}

static mrb_value
mrb_layer_set_opacity(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  tmx_layer *layer = DATA_PTR(self);
  mrb_get_args(mrb, "f", &value);
  layer->opacity = tilemap_clamp(value, 0, 255);
  return mrb_float_value(mrb, value);
}

static mrb_value
mrb_layer_update(mrb_state *mrb, mrb_value self)
{
  mrb_float dt;
  mrb_get_args(mrb, "f", &dt);
  return self;
}

static mrb_value
mrb_layer_draw(mrb_state *mrb, mrb_value self)
{
  return self;
}

static mrb_value
mrb_layer_group_draw(mrb_state *mrb, mrb_value self)
{
  mrb_value map = mrb_layer_get_map(mrb, self);
  tmx_map *map_data = mrb_carbuncle_get_tilemap(mrb, map);
  tmx_layer *layer = DATA_PTR(self);
  if (layer)
  {
    Vector2 scale = { 1, 1 };
    tmx_property *sx = tmx_get_property(layer->properties, "Carbuncle Offset Scale X");
    tmx_property *sy = tmx_get_property(layer->properties, "Carbuncle Offset Scale Y");
    if (sx && sx->type == PT_FLOAT) { scale.x = sx->value.decimal; }
    if (sy && sy->type == PT_FLOAT) { scale.y = sy->value.decimal; }    
    Vector2 *offset = mrb_carbuncle_get_point(mrb, mrb_iv_get(mrb, map, OFFSET_SYMBOL));
    Vector2 round_offset = { floorf(scale.x * offset->x), floorf(scale.y * offset->y) };
    draw_all_layers(&round_offset, map_data, layer->content.group_head);
  }
  return self;
}

static mrb_value
mrb_image_layer_draw(mrb_state *mrb, mrb_value self)
{
  mrb_value map = mrb_layer_get_map(mrb, self);
  tmx_layer *layer = DATA_PTR(self);
  if (layer)
  {
    Vector2 scale = { 1, 1 };
    tmx_property *sx = tmx_get_property(layer->properties, "Carbuncle Offset Scale X");
    tmx_property *sy = tmx_get_property(layer->properties, "Carbuncle Offset Scale Y");
    if (sx && sx->type == PT_FLOAT) { scale.x = sx->value.decimal; }
    if (sy && sy->type == PT_FLOAT) { scale.y = sy->value.decimal; }    
    Vector2 *offset = mrb_carbuncle_get_point(mrb, mrb_iv_get(mrb, map, OFFSET_SYMBOL));
    Vector2 round_offset = { floorf(scale.x * offset->x), floorf(scale.y * offset->y) };
    draw_image_layer(&round_offset, layer, layer->content.image);
  }
  return self;
}

static mrb_value
mrb_object_layer_draw(mrb_state *mrb, mrb_value self)
{
  mrb_value map = mrb_layer_get_map(mrb, self);
  tmx_layer *layer = DATA_PTR(self);
  if (layer)
  {
    Vector2 scale = { 1, 1 };
    tmx_property *sx = tmx_get_property(layer->properties, "Carbuncle Offset Scale X");
    tmx_property *sy = tmx_get_property(layer->properties, "Carbuncle Offset Scale Y");
    if (sx && sx->type == PT_FLOAT) { scale.x = sx->value.decimal; }
    if (sy && sy->type == PT_FLOAT) { scale.y = sy->value.decimal; }    
    Vector2 *offset = mrb_carbuncle_get_point(mrb, mrb_iv_get(mrb, map, OFFSET_SYMBOL));
    Vector2 round_offset = { floorf(scale.x * offset->x), floorf(scale.y * offset->y) };
    draw_objects(&round_offset, layer->content.objgr);
  }
  return self;
}

static mrb_value
mrb_tile_layer_draw(mrb_state *mrb, mrb_value self)
{
  mrb_value map = mrb_layer_get_map(mrb, self);
  tmx_map *map_data = mrb_carbuncle_get_tilemap(mrb, map);
  tmx_layer *layer = DATA_PTR(self);
  if (layer)
  {
    Vector2 scale = { 1, 1 };
    tmx_property *sx = tmx_get_property(layer->properties, "Carbuncle Offset Scale X");
    tmx_property *sy = tmx_get_property(layer->properties, "Carbuncle Offset Scale Y");
    if (sx && sx->type == PT_FLOAT) { scale.x = sx->value.decimal; }
    if (sy && sy->type == PT_FLOAT) { scale.y = sy->value.decimal; }    
    Vector2 *offset = mrb_carbuncle_get_point(mrb, mrb_iv_get(mrb, map, OFFSET_SYMBOL));
    Vector2 round_offset = { floorf(scale.x * offset->x), floorf(scale.y * offset->y) };
    draw_tile_layer(&round_offset, map_data, layer);
  }
  return self;
}

static mrb_value
mrb_any_get_properties(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, PROPERTIES_SYMBOL);
}

static mrb_value
mrb_tilemap_get_tiles(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, TILES_SYMBOL);
}

static mrb_value
mrb_tile_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value map;
  mrb_get_args(mrb, "o", &map);
  mrb_carbuncle_get_tilemap(mrb, map);
  mrb_iv_set(mrb, self, MAP_SYMBOL, map);
  return self;
}

static mrb_value
mrb_tile_layer_tile_at(mrb_state *mrb, mrb_value self)
{
  mrb_int x, y;
  mrb_value map = mrb_layer_get_map(mrb, self);
  tmx_map *map_data = mrb_carbuncle_get_tilemap(mrb, map);
  tmx_layer *layer = DATA_PTR(self);
  mrb_get_args(mrb, "ii", &x, &y);
  if (x >= 0 && x < map_data->width && y >= 0 && y < map_data->height)
  {
    mrb_value tiles = mrb_iv_get(mrb, map, TILES_SYMBOL);
    mrb_int i = x + y * map_data->width;
    return mrb_ary_entry(tiles, layer->content.gids[i]);
  }
  return mrb_nil_value();
}

static mrb_value
mrb_tile_get_id(mrb_state *mrb, mrb_value self)
{
  tmx_tile *tile = DATA_PTR(self);
  return mrb_fixnum_value(tile->id);
}

void
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
  mrb_define_method(mrb, tilemap, "properties", mrb_any_get_properties, MRB_ARGS_NONE());
  mrb_define_method(mrb, tilemap, "offset", mrb_tilemap_get_offset, MRB_ARGS_NONE());
  mrb_define_method(mrb, tilemap, "width", mrb_tilemap_get_width, MRB_ARGS_NONE());
  mrb_define_method(mrb, tilemap, "height", mrb_tilemap_get_height, MRB_ARGS_NONE());
  mrb_define_method(mrb, tilemap, "tile_width", mrb_tilemap_get_tile_width, MRB_ARGS_NONE());
  mrb_define_method(mrb, tilemap, "tile_height", mrb_tilemap_get_tile_height, MRB_ARGS_NONE());

  mrb_define_method(mrb, tilemap, "offset=", mrb_tilemap_set_offset, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, tilemap, "update", mrb_tilemap_update, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, tilemap, "draw", mrb_tilemap_draw, MRB_ARGS_NONE());

  {
    struct RClass *layer = mrb_define_class_under(mrb, tilemap, "Layer", mrb->object_class);
    MRB_SET_INSTANCE_TT(layer, MRB_TT_DATA);

    mrb_define_method(mrb, layer, "initialize", mrb_layer_initialize, MRB_ARGS_REQ(1));

    mrb_define_method(mrb, layer, "visible?", mrb_layer_get_visible, MRB_ARGS_NONE());

    mrb_define_method(mrb, layer, "properties", mrb_any_get_properties, MRB_ARGS_NONE());
    mrb_define_method(mrb, layer, "map", mrb_layer_get_map, MRB_ARGS_NONE());
    mrb_define_method(mrb, layer, "visible", mrb_layer_get_visible, MRB_ARGS_NONE());
    mrb_define_method(mrb, layer, "opacity", mrb_layer_get_opacity, MRB_ARGS_NONE());

    mrb_define_method(mrb, layer, "visible=", mrb_layer_set_visible, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, layer, "opacity=", mrb_layer_set_opacity, MRB_ARGS_REQ(1));

    mrb_define_method(mrb, layer, "update", mrb_layer_update, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, layer, "draw", mrb_layer_draw, MRB_ARGS_NONE());

    {
      struct RClass *group = mrb_define_class_under(mrb, tilemap, "LayerGroup", layer);
      MRB_SET_INSTANCE_TT(group, MRB_TT_DATA);
      mrb_define_method(mrb, group, "draw", mrb_layer_group_draw, MRB_ARGS_NONE());
    }
    {
      struct RClass *img = mrb_define_class_under(mrb, tilemap, "ImageLayer", layer);
      MRB_SET_INSTANCE_TT(img, MRB_TT_DATA);
      mrb_define_method(mrb, img, "draw", mrb_image_layer_draw, MRB_ARGS_NONE());
    }
    {
      struct RClass *obj = mrb_define_class_under(mrb, tilemap, "ObjectLayer", layer);
      MRB_SET_INSTANCE_TT(obj, MRB_TT_DATA);
      mrb_define_method(mrb, obj, "draw", mrb_object_layer_draw, MRB_ARGS_NONE());
    }
    {
      struct RClass *tiles = mrb_define_class_under(mrb, tilemap, "TileLayer", layer);
      MRB_SET_INSTANCE_TT(tiles, MRB_TT_DATA);
      mrb_define_method(mrb, tiles, "tile_at", mrb_tile_layer_tile_at, MRB_ARGS_REQ(2));
      mrb_define_method(mrb, tiles, "draw", mrb_tile_layer_draw, MRB_ARGS_NONE());
    }
  }
  {
    struct RClass *tile = mrb_define_class_under(mrb, tilemap, "Tile", mrb->object_class);
    MRB_SET_INSTANCE_TT(tile, MRB_TT_DATA);

    mrb_define_method(mrb, tile, "initialize", mrb_tile_initialize, MRB_ARGS_REQ(1));

    mrb_define_method(mrb, tile, "map", mrb_layer_get_map, MRB_ARGS_NONE());
    mrb_define_method(mrb, tile, "id", mrb_tile_get_id, MRB_ARGS_NONE());
  }
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
