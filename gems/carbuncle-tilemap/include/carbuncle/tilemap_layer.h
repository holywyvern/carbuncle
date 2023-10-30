#ifndef CARBUNCLE_TILEMAP_LAYER_H
#define CARBUNCLE_TILEMAP_LAYER_H

#include <mruby.h>
#include <tmx.h>

#ifdef __cplusplus
extern "C" {
#endif

void
mrb_init_carbuncle_tilemap_layer(mrb_state *mrb, struct RClass *tilemap);

#ifdef __cplusplus
}
#endif

#endif