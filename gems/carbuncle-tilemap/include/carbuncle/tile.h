#ifndef CARBUNCLE_TILE_H
#define CARBUNCLE_TILE_H

#include <mruby.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Tile
{
  mrb_int id;
} Tile;

void
mrb_init_carbuncle_tile(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif