#ifndef CARBUNCLE_SERVER_H
#define CARBUNCLE_SERVER_H

#include <mruby.h>

#ifdef __cplusplus
extern "C" {
#endif

void
mrb_init_carbuncle_server(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif
