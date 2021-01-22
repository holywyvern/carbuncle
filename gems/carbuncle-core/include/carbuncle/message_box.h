#ifndef CARBUNCLE_MESSAGE_BOX_H
#define CARBUNCLE_MESSAGE_BOX_H

#include <mruby.h>

#ifdef __cplusplus
extern "C" {
#endif

void
mrb_init_carbuncle_messagebox(mrb_state *mrb);

void
mrb_carbuncle_show_fatal(const char *title, const char *message);

void
mrb_carbuncle_show_error(mrb_state *mrb, mrb_value error);

#ifdef __cplusplus
}
#endif

#endif
