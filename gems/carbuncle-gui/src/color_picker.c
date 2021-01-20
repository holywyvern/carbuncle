#include "carbuncle/core.h"
#include "carbuncle/font.h"
#include "carbuncle/rect.h"
#include "carbuncle/point.h"
#include "carbuncle/color.h"

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
gui_color_picker(mrb_state *mrb, mrb_value self)
{
  mrb_value value;
  mrb_int format;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "oi", &value, &format);
  Color *color_data = mrb_carbuncle_get_color(mrb, value);
  struct nk_colorf color = (struct nk_colorf){
    (float)color_data->r / 255.0f, (float)color_data->g / 255.0f,
    (float)color_data->b / 255.0f, (float)color_data->a / 255.0f
  };
  if (nk_color_pick(&(ctx->nk), &color, format)) {
    *color_data = (Color) {
      color.r * 255, color.g * 255, color.b * 255, color.a * 255
    };
  }
  return value;
}

void
mrb_init_carbuncle_gui_color_picker(mrb_state *mrb, struct RClass *gui)
{
  mrb_define_method(mrb, gui, "color_picker", gui_color_picker, MRB_ARGS_REQ(2));

  mrb_define_const(mrb, gui, "RGB", mrb_fixnum_value(NK_RGB));
  mrb_define_const(mrb, gui, "RGBA", mrb_fixnum_value(NK_RGBA));
}
