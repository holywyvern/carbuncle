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
gui_combo(mrb_state *mrb, mrb_value self)
{
  Vector2 *point;
  const char *selected;
  mrb_value point_obj, block;
  struct nk_vec2 size;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "zo&", &selected, &point_obj, &block);
  point = mrb_carbuncle_get_point(mrb, point_obj);
  size = (struct nk_vec2){point->x, point->y};
  if (nk_combo_begin_label(&(ctx->nk), selected, size)) {
    nk_layout_row_dynamic(&(ctx->nk), size.y, 1);
    mrb_yield_argv(mrb, block, 0, NULL);
    nk_combo_end(&(ctx->nk));
  }
  return self;
}

static mrb_value
gui_combo_item_text(mrb_state *mrb, mrb_value self)
{
  const char *title;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "z", &title);
  return mrb_bool_value(nk_combo_item_label(&(ctx->nk), title, NK_LEFT));
}

void
mrb_init_carbuncle_gui_combo(mrb_state *mrb, struct RClass *gui)
{
  mrb_define_method(mrb, gui, "combo", gui_combo, MRB_ARGS_REQ(2)|MRB_ARGS_BLOCK());
  mrb_define_method(mrb, gui, "combo_item_text", gui_combo_item_text, MRB_ARGS_REQ(1));
}
