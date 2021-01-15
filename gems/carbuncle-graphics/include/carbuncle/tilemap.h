#ifndef CARBUNCLE_TILEMAP_H
#define CARBUNCLE_TILEMAP_H

#include <mruby.h>
#include <tmx.h>

#ifdef __cplusplus
extern "C" {
#endif

void
mrb_init_carbuncle_tilemap(mrb_state *mrb);

tmx_map *
mrb_carbuncle_get_tilemap(mrb_state *mrb, mrb_value obj);

mrb_bool
mrb_carbuncle_tilemap_p(mrb_value obj);

#ifdef __cplusplus
}
#endif

#endif
