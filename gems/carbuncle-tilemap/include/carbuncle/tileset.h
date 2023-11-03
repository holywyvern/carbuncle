#ifndef CARBUNCLE_TILESET_H
#define CARBUNCLE_TILESET_H

#include <mruby.h>

#include "raylib.h"

#include "carbuncle/tile.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Tileset
{
  Tile    *tiles;
  mrb_int  tile_count;
  Texture *texture;
  mrb_int  texture_count;
  mrb_int  tile_width;
  mrb_int  tile_height;
} Tileset;

void
mrb_init_carbuncle_tileset(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif