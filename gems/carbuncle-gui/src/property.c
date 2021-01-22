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
gui_int(mrb_state *mrb, mrb_value self)
{
  const char *name;
  mrb_int value, min, max, step;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);  
  mrb_get_args(mrb, "ziiii", &name, &value, &min, &max, &step);
  value = nk_propertyi(&(ctx->nk), name, min, value, max, step, step);
  return mrb_float_value(mrb, value);
}

static mrb_value
gui_float(mrb_state *mrb, mrb_value self)
{
  const char *name;
  mrb_float value, min, max, step;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);  
  mrb_get_args(mrb, "zffff", &name, &value, &min, &max, &step);
  value = nk_propertyf(&(ctx->nk), name, min, value, max, step, step);
  return mrb_float_value(mrb, value);
}

void
mrb_init_carbuncle_gui_property(mrb_state *mrb, struct RClass *gui)
{
  mrb_define_method(mrb, gui, "int", gui_int, MRB_ARGS_REQ(6));
  mrb_define_method(mrb, gui, "float", gui_float, MRB_ARGS_REQ(6));
}