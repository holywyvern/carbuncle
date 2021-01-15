#ifndef CARBUNCLE_NUKLEAR_CONFIG_H
#define CARBUNCLE_NUKLEAR_CONFIG_H 1

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_COMMAND_USERDATA
#define NK_KEYSTATE_BASED_INPUT
#define NK_INCLUDE_FIXED_TYPES

#include "nuklear.h"
#include "mruby.h"
#include "raylib.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mrb_GuiContext
{
  struct nk_context   nk;
  struct nk_user_font font;
  Vector2 mouse;
  mrb_bool scissor;
};

struct mrb_GuiContext *
mrb_carbuncle_gui_get_context(mrb_state *mrb, mrb_value self);

#ifdef __cplusplus
}
#endif

#endif /* CARBUNCLE_NUKLEAR_CONFIG_H */
