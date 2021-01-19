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
gui_button(mrb_state *mrb, mrb_value self)
{
  nk_bool result;
  const char *title;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "z", &title);
  result = nk_button_label(&(ctx->nk), title);
  return mrb_bool_value(result);
}

void
mrb_init_carbuncle_gui_button(mrb_state *mrb, struct RClass *gui)
{
  mrb_define_method(mrb, gui, "button", gui_button, MRB_ARGS_REQ(1));
}
