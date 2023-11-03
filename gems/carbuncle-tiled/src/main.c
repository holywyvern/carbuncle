#include <mruby.h>

#include "carbuncle/tilemap.h"
#include "carbuncle/tilemap_layer.h"
#include "carbuncle/tilemap_tile.h"

void
mrb_carbuncle_tiled_gem_init(mrb_state *mrb)
{
  struct RClass *tilemap = mrb_init_carbuncle_tilemap(mrb);
  mrb_init_carbuncle_tilemap_layer(mrb, tilemap);
  mrb_init_carbuncle_tilemap_tile(mrb, tilemap);
}

void
mrb_carbuncle_tiled_gem_final(mrb_state *mrb)
{
  /* finalizer */
}
