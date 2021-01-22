#include "carbuncle/core.h"
#include "carbuncle/font.h"
#include "carbuncle/rect.h"
#include "carbuncle/point.h"

#include <carbuncle/nuklear_config.h>
#include <nuklear.h>

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include <mruby/error.h>
#include <mruby/array.h>

#include <raylib.h>

static mrb_value
gui_tree(mrb_state *mrb, mrb_value self)
{
  mrb_int state_value, type;
  const char *title;
  mrb_value block;
  enum nk_collapse_states state;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "zii&", &title, &type, &state_value, &block);
  state = state_value;
  if (nk_tree_state_push(&(ctx->nk), type, title, &state)) {
    mrb_yield_argv(mrb, block, 0, NULL);
    nk_tree_pop(&(ctx->nk));
  }
  return mrb_fixnum_value(state);
}

void
mrb_init_carbuncle_gui_tree(mrb_state *mrb, struct RClass *gui)
{
  mrb_define_method(mrb, gui, "tree", gui_tree, MRB_ARGS_REQ(3)|MRB_ARGS_BLOCK());

  mrb_define_const(mrb, gui, "TREE_TAB", mrb_fixnum_value(NK_TREE_TAB));
  mrb_define_const(mrb, gui, "TREE_NODE", mrb_fixnum_value(NK_TREE_NODE));

  mrb_define_const(mrb, gui, "MINIMIZED", mrb_fixnum_value(NK_MINIMIZED));
  mrb_define_const(mrb, gui, "MAXIMIZED", mrb_fixnum_value(NK_MAXIMIZED));
}
