#ifndef CARBUNCLE_TILE_LAYER_H
#define CARBUNCLE_TILE_LAYER_H

#include <mruby.h>

#include "carbuncle/tileset.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TileLayer
{
  Tileset *tileset;
  mrb_int  width;
  mrb_int  height;
  mrb_int *tile_ids;
} TileLayer;

void
mrb_init_carbuncle_tile_layer(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif