#include <mruby.h>

#include "carbuncle/tile.h"
#include "carbuncle/tileset.h"
#include "carbuncle/tile_layer.h"

void
mrb_carbuncle_tilemap_gem_init(mrb_state *mrb)
{
  mrb_init_carbuncle_tile(mrb);
  mrb_init_carbuncle_tile_layer(mrb);
  mrb_init_carbuncle_tileset(mrb);
}

void
mrb_carbuncle_tilemap_gem_final(mrb_state *mrb)
{
  /* finalizer */
}
