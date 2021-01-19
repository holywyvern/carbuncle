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

static struct nk_color
to_nk_color(Color *color)
{
  return (struct nk_color) {
    color->r, color->g, color->b, color->a
  };
}

static mrb_value
draw_chart(mrb_state *mrb, mrb_value self, enum nk_chart_type type)
{
  mrb_value mrb_color, mrb_highlight, block;
  mrb_int items;
  mrb_float min, max;
  struct nk_color color, highlight;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "ooiff&", &mrb_color, &mrb_highlight, &items, &min, &max, &block);
  color = to_nk_color(mrb_carbuncle_get_color(mrb, mrb_color));
  highlight = to_nk_color(mrb_carbuncle_get_color(mrb, mrb_highlight));
  if (nk_chart_begin_colored(&(ctx->nk), type, color, highlight, items, min, max))
  {
    mrb_yield_argv(mrb, block, 0, NULL);
    nk_chart_end(&(ctx->nk));
  }
  return self;
}

static mrb_value
gui_line_chart(mrb_state *mrb, mrb_value self)
{
  return draw_chart(mrb, self, NK_CHART_LINES);;
}

static mrb_value
gui_column_chart(mrb_state *mrb, mrb_value self)
{
  return draw_chart(mrb, self, NK_CHART_COLUMN);
}

static mrb_value
gui_max_chart(mrb_state *mrb, mrb_value self)
{
  return draw_chart(mrb, self, NK_CHART_MAX);
}

static mrb_value
gui_chart_push(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "f", &value);
  nk_chart_push(&(ctx->nk), value);
  return self;
}

void
mrb_init_carbuncle_gui_chart(mrb_state *mrb, struct RClass *gui)
{

  mrb_define_method(mrb, gui, "line_chart", gui_line_chart, MRB_ARGS_REQ(5)|MRB_ARGS_BLOCK());
  mrb_define_method(mrb, gui, "column_chart", gui_column_chart, MRB_ARGS_REQ(5)|MRB_ARGS_BLOCK());
  mrb_define_method(mrb, gui, "max_chart", gui_max_chart, MRB_ARGS_REQ(5)|MRB_ARGS_BLOCK());

  mrb_define_method(mrb, gui, "chart_push", gui_chart_push, MRB_ARGS_REQ(1));
}
