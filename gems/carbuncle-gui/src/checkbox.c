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
gui_checkbox(mrb_state *mrb, mrb_value self)
{
  mrb_bool mrb_active;
  const char *label;
  nk_bool active;
  mrb_get_args(mrb, "zb", &label, &mrb_active);
  active = mrb_active;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  nk_checkbox_label(&(ctx->nk), label, &active);
  return mrb_bool_value(active);
}

void
mrb_init_carbuncle_gui_checkbox(mrb_state *mrb, struct RClass *gui)
{
  mrb_define_method(mrb, gui, "checkbox", gui_checkbox, MRB_ARGS_REQ(1));
}
