#ifndef CARBUNCLE_TILEMAP_TILE_H
#define CARBUNCLE_TILEMAP_TILE_H

#include <mruby.h>
#include <tmx.h>

#ifdef __cplusplus
extern "C" {
#endif

void
mrb_init_carbuncle_tilemap_tile(mrb_state *mrb, struct RClass *tilemap);

#ifdef __cplusplus
}
#endif

#endif