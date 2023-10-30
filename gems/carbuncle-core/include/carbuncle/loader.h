#ifndef CARBUNCLE_LOADER_H
#define CARBUNCLE_LOADER_H

#include <mruby.h>

#ifdef __cplusplus
extern "C" {
#endif

void
mrb_init_carbuncle_loader(mrb_state *mrb);

mrb_bool
mrb_carbuncle_loader_loading_p(mrb_state *mrb);

mrb_bool
mrb_carbuncle_loader_error_p(mrb_state *mrb);

mrb_float
mrb_carbuncle_loader_rate(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif