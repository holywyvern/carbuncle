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
mrb_gui_dynamic_layout(mrb_state *mrb, mrb_value self)
{
  mrb_int height, columns;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "ii", &height, &columns);
  nk_layout_row_dynamic(&(ctx->nk), height, columns);
  return self;
}

static mrb_value
mrb_gui_static_layout(mrb_state *mrb, mrb_value self)
{
  mrb_int height, width, columns;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "iii", &height, &width, &columns);
  nk_layout_row_static(&(ctx->nk), height, width, columns);
  return self;
}

static mrb_value
mrb_gui_custom_layout(mrb_state *mrb, mrb_value self)
{
  mrb_bool layout_static;
  mrb_float height;
  mrb_int columns;
  mrb_value block;  
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "bfi&", &layout_static, &height, &columns, &block);
  nk_layout_row_begin(&(ctx->nk), layout_static ? NK_STATIC : NK_DYNAMIC, height, columns);
  mrb_yield_argv(mrb, block, 0, NULL);
  nk_layout_row_end(&(ctx->nk));
  return self;
}

static mrb_value
mrb_gui_push_column(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "f", &value);
  nk_layout_row_push(&(ctx->nk), value);
  return self;
}

static mrb_value
mrb_gui_space_layout(mrb_state *mrb, mrb_value self)
{
  mrb_int count;
  mrb_bool layout_static;
  mrb_float height;
  mrb_value block;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "bfi&", &layout_static, &height, &count, &block);
  nk_layout_space_begin(&(ctx->nk), layout_static ? NK_STATIC : NK_DYNAMIC, height, count);
  mrb_yield_argv(mrb, block, 0, NULL);
  nk_layout_space_end(&(ctx->nk));
  return self;
}

static mrb_value
mrb_gui_space_push(mrb_state *mrb, mrb_value self)
{
  mrb_value rect;
  Rectangle *data;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "o", &rect);
  data = mrb_carbuncle_get_rect(mrb, rect);
  nk_layout_space_push(&(ctx->nk), (struct nk_rect){ data->x, data->y, data->width, data->height });
  return self;
}

static mrb_value
mrb_gui_flex_layout(mrb_state *mrb, mrb_value self)
{
  mrb_float height;
  mrb_value block;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "f&", &height, &block);
  nk_layout_row_template_begin(&(ctx->nk), height);
  mrb_yield_argv(mrb, block, 0, NULL);
  nk_layout_row_template_end(&(ctx->nk));
  return self;
}

static mrb_value
mrb_gui_dynamic_column(mrb_state *mrb, mrb_value self)
{
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  nk_layout_row_template_push_dynamic(&(ctx->nk));
  return self;
}

static mrb_value
mrb_gui_variable_column(mrb_state *mrb, mrb_value self)
{
  mrb_float min_width;
  mrb_get_args(mrb, "f", &min_width);
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  nk_layout_row_template_push_variable(&(ctx->nk), min_width);
  return self;
}

static mrb_value
mrb_gui_static_column(mrb_state *mrb, mrb_value self)
{
  mrb_float width;
  mrb_get_args(mrb, "f", &width);
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  nk_layout_row_template_push_static(&(ctx->nk), width);
  return self;
}

void
mrb_init_carbuncle_gui_layout(mrb_state *mrb, struct RClass *gui)
{
  mrb_define_method(mrb, gui, "dynamic_layout", mrb_gui_dynamic_layout, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, gui, "static_layout", mrb_gui_static_layout, MRB_ARGS_REQ(3));

  mrb_define_method(mrb, gui, "custom_layout", mrb_gui_custom_layout, MRB_ARGS_REQ(3)|MRB_ARGS_BLOCK());
  mrb_define_method(mrb, gui, "push_column", mrb_gui_push_column, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, gui, "flex_layout", mrb_gui_flex_layout, MRB_ARGS_REQ(1)|MRB_ARGS_BLOCK());
  mrb_define_method(mrb, gui, "dynamic_column", mrb_gui_dynamic_column, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, gui, "variable_column", mrb_gui_variable_column, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, gui, "static_column", mrb_gui_static_column, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, gui, "space_layout", mrb_gui_space_layout, MRB_ARGS_REQ(3)|MRB_ARGS_BLOCK());
  mrb_define_method(mrb, gui, "space_push", mrb_gui_space_push, MRB_ARGS_REQ(1));
}