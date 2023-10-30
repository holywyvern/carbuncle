#ifndef CARBUNCLE_TILEMAP_H
#define CARBUNCLE_TILEMAP_H

#include <mruby.h>
#include <tmx.h>
#include "raylib.h"

#define LAYERS_SYMBOL     mrb_intern_cstr(mrb, "#layers")
#define MAP_SYMBOL        mrb_intern_cstr(mrb, "#map")
#define OFFSET_SYMBOL     mrb_intern_cstr(mrb, "#offset")
#define PROPERTIES_SYMBOL mrb_intern_cstr(mrb, "#properties")
#define TILES_SYMBOL      mrb_intern_cstr(mrb, "#tiles")

#ifdef __cplusplus
extern "C" {
#endif

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

struct RClass *
mrb_init_carbuncle_tilemap(mrb_state *mrb);

tmx_map *
mrb_carbuncle_get_tilemap(mrb_state *mrb, mrb_value obj);

mrb_bool
mrb_carbuncle_tilemap_p(mrb_value obj);

void
mrb_carbuncle_tilemap_utils_draw_tile(
  Vector2 *offset, void *image,
  unsigned int sx, unsigned int sy,
  unsigned int sw, unsigned int sh,
  unsigned int dx, unsigned int dy,
  float opacity, unsigned int flags
);

void
mrb_carbuncle_tilemap_utils_draw_tile_layer(Vector2 *offset, tmx_map *map, tmx_layer *layer);

void
mrb_carbuncle_tilemap_utils_draw_objects(Vector2 *offset, tmx_object_group *objgr);

void
mrb_carbuncle_tilemap_utils_draw_polygon(Vector2 *offset, Color *color, double **points, double x, double y, size_t size);

void
mrb_carbuncle_tilemap_utils_draw_polyline(Vector2 *offset, Color *color, double **points, double x, double y, size_t size);

void
mrb_carbuncle_tilemap_utils_draw_all_layers(Vector2 *offset, tmx_map *map, tmx_layer *layer);

void
mrb_carbuncle_tilemap_utils_draw_layer(Vector2 *offset, tmx_map *map, tmx_layer *layer);

void
mrb_carbuncle_tilemap_utils_draw_image_layer(Vector2 *offset, tmx_layer *layer, tmx_image *image);

#ifdef __cplusplus
}
#endif

#endif
