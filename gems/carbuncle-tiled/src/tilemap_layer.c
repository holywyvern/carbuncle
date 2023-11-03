#include "carbuncle/core.h"
#include "carbuncle/point.h"
#include "carbuncle/color.h"
#include "carbuncle/tilemap.h"
#include "carbuncle/tilemap_layer.h"
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

static inline mrb_float
tilemap_clamp(mrb_float value, mrb_float min, mrb_float max)
{
  if (value < min) { return min; }
  if (value > max) { return max; }
  return value;
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
mrb_layer_get_properties(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, PROPERTIES_SYMBOL);
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
    mrb_carbuncle_tilemap_utils_draw_all_layers(&round_offset, map_data, layer->content.group_head);
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
    mrb_carbuncle_tilemap_utils_draw_image_layer(&round_offset, layer, layer->content.image);
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
    mrb_carbuncle_tilemap_utils_draw_objects(&round_offset, layer->content.objgr);
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
    mrb_carbuncle_tilemap_utils_draw_tile_layer(&round_offset, map_data, layer);
  }
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

void
mrb_init_carbuncle_tilemap_layer(mrb_state *mrb, struct RClass *tilemap)
{
  struct RClass *layer = mrb_define_class_under(mrb, tilemap, "Layer", mrb->object_class);
  MRB_SET_INSTANCE_TT(layer, MRB_TT_DATA);

  mrb_define_method(mrb, layer, "initialize", mrb_layer_initialize, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, layer, "visible?", mrb_layer_get_visible, MRB_ARGS_NONE());

  mrb_define_method(mrb, layer, "properties", mrb_layer_get_properties, MRB_ARGS_NONE());
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