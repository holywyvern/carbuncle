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

void
mrb_carbuncle_tilemap_utils_draw_layer(Vector2 *offset, tmx_map *map, tmx_layer *layer)
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
        mrb_carbuncle_tilemap_utils_draw_all_layers(&layer_offset, map, layer->content.group_head);
        break;
      }
      case L_OBJGR:
      {
        mrb_carbuncle_tilemap_utils_draw_objects(&layer_offset, layer->content.objgr);
        break;
      }
      case L_IMAGE:
      {
        mrb_carbuncle_tilemap_utils_draw_image_layer(&layer_offset, layer, layer->content.image);
        break;
      }
      case L_LAYER:
      {
        mrb_carbuncle_tilemap_utils_draw_tile_layer(&layer_offset, map, layer);
        break;
      }
      default: { break; }
    }
  }
}

void
mrb_carbuncle_tilemap_utils_draw_image_layer(Vector2 *offset, tmx_layer *layer, tmx_image *image)
{
  Texture2D *texture = (Texture2D*)image->resource_image;
  DrawTexture(*texture, layer->offsetx - offset->x, layer->offsety - offset->y, WHITE);
}

void
mrb_carbuncle_tilemap_utils_draw_all_layers(Vector2 *offset, tmx_map *map, tmx_layer *layer)
{
  while (layer)
  {
    mrb_carbuncle_tilemap_utils_draw_layer(offset, map, layer);
    layer = layer->next;
  }
}


void
mrb_carbuncle_tilemap_utils_draw_polyline(Vector2 *offset, Color *color, double **points, double x, double y, size_t size)
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

void
mrb_carbuncle_tilemap_utils_draw_polygon(Vector2 *offset, Color *color, double **points, double x, double y, size_t size)
{
  mrb_carbuncle_tilemap_utils_draw_polyline(offset, color, points, x, y, size);
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

void
mrb_carbuncle_tilemap_utils_draw_objects(Vector2 *offset, tmx_object_group *objgr) {
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
          mrb_carbuncle_tilemap_utils_draw_polygon(offset, &color, head->content.shape->points, head->x, head->y, head->content.shape->points_len);
          break;
        }
        case OT_POLYLINE:
        {
          mrb_carbuncle_tilemap_utils_draw_polyline(offset, &color, head->content.shape->points, head->x, head->y, head->content.shape->points_len);
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

void
mrb_carbuncle_tilemap_utils_draw_tile(Vector2 *offset, void *image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
               unsigned int dx, unsigned int dy, float opacity, unsigned int flags)
{
  Rectangle src = (Rectangle) {sx, sy, sw, sh};
  Vector2 pos = (Vector2) {dx - offset->x, dy - offset->y};
  Texture2D *img = image;
  Color color = (Color) {opacity, opacity, opacity, opacity};
  if(img) {
    DrawTextureRec(*img, src, pos, color);
  }
}

void
mrb_carbuncle_tilemap_utils_draw_tile_layer(Vector2 *offset, tmx_map *map, tmx_layer *layer)
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
      gid = (layer->content.gids[(i*map->width)+j]) & TMX_FLIP_BITS_REMOVAL;
      tile = map->tiles[gid];
      
      if (tile && tile->animation_len)
      {
        struct mrb_TileData *data = tile->user_data.pointer;
        if(data) 
        {
          tmx_anim_frame animation = tile->animation[data->frame];
          tile = map->tiles[animation.tile_id + 1];
        }
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
        flags = (layer->content.gids[(i*map->width)+j]) & ~TMX_FLIP_BITS_REMOVAL;
        mrb_carbuncle_tilemap_utils_draw_tile(offset, image, x, y, w, h, j * ts->tile_width, i * ts->tile_height, op, flags);
      }
    }
  }
}