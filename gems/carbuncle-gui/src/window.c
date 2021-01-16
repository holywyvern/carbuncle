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

#define WINDOW_KEYS 14

static const char *WINDOW_KEYWORDS[WINDOW_KEYS] = {
  "id", "border", "movable", "scalable", "closable", "minimizable",
  "disable_scrollbar", "title", "auto_scrollbar", "background", "scale_left", "disable",
  "minimized", "maximized"
};

static mrb_value
draw_window(mrb_state *mrb, mrb_value self)
{
  return mrb_yield_argv(mrb, self, 0, NULL);
}

static mrb_value
mrb_gui_window(mrb_state *mrb, mrb_value self)
{
  mrb_bool raised;
  nk_flags flags;
  const char *id, *title_str;
  mrb_value kw_values[WINDOW_KEYS];
  mrb_value title, rect, block, result;
  struct nk_rect bounds;
  Rectangle *rect_data;
  const mrb_kwargs kwargs = { WINDOW_KEYS, kw_values, WINDOW_KEYWORDS, 0, NULL };
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "So:&", &title, &rect, &kwargs, &block);
  rect_data = mrb_carbuncle_get_rect(mrb, rect);
  if (mrb_undef_p(kw_values[0])) { kw_values[0] = title; }
  if (mrb_carbuncle_test(kw_values[1]))  { flags |= NK_WINDOW_BORDER; }
  if (mrb_carbuncle_test(kw_values[2]))  { flags |= NK_WINDOW_MOVABLE; }
  if (mrb_carbuncle_test(kw_values[3]))  { flags |= NK_WINDOW_SCALABLE; }
  if (mrb_carbuncle_test(kw_values[4]))  { flags |= NK_WINDOW_CLOSABLE; }
  if (mrb_carbuncle_test(kw_values[5]))  { flags |= NK_WINDOW_MINIMIZABLE; }
  if (mrb_carbuncle_test(kw_values[6]))  { flags |= NK_WINDOW_NO_SCROLLBAR; }
  if (mrb_carbuncle_test(kw_values[7]))  { flags |= NK_WINDOW_TITLE; }
  if (mrb_carbuncle_test(kw_values[8]))  { flags |= NK_WINDOW_SCROLL_AUTO_HIDE; }
  if (mrb_carbuncle_test(kw_values[9]))  { flags |= NK_WINDOW_BACKGROUND; }
  if (mrb_carbuncle_test(kw_values[10])) { flags |= NK_WINDOW_SCALE_LEFT; }
  if (mrb_carbuncle_test(kw_values[11])) { flags |= NK_WINDOW_NO_INPUT; }
  if (mrb_carbuncle_test(kw_values[12])) { flags |= NK_MINIMIZED; }
  if (mrb_carbuncle_test(kw_values[13])) { flags |= NK_MAXIMIZED; }
  title_str = mrb_string_cstr(mrb, title);
  id = mrb_string_cstr(mrb, kw_values[0]);
  bounds.x = rect_data->x;
  bounds.y = rect_data->y;
  bounds.w = rect_data->width;
  bounds.h = rect_data->height;
  nk_bool ok = nk_begin_titled(&(ctx->nk), title_str, id, bounds, flags);
  result = mrb_protect(mrb, draw_window, block, &raised);
  nk_end(&(ctx->nk));
  if (raised) { mrb_exc_raise(mrb, result); }
  return mrb_bool_value(ok);
}

static mrb_value
mrb_gui_window_bounds(mrb_state *mrb, mrb_value self)
{
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  struct nk_rect bounds = nk_window_get_bounds(&(ctx->nk));
  return mrb_carbuncle_rect_new(mrb, bounds.x, bounds.y, bounds.w, bounds.h);
}

static mrb_value
mrb_gui_window_set_bounds(mrb_state *mrb, mrb_value self)
{
  const char *name;
  mrb_value rect;
  Rectangle *data;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "zo", &name, &rect);
  data = mrb_carbuncle_get_rect(mrb, rect);
  nk_window_set_bounds(&(ctx->nk), name, (struct nk_rect){ data->x, data->y, data->width, data->height });
  return self;
}

static mrb_value
mrb_gui_window_focusQ(mrb_state *mrb, mrb_value self)
{
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  return mrb_bool_value(nk_window_has_focus(&(ctx->nk)));
}

static mrb_value
mrb_gui_window_minimizedQ(mrb_state *mrb, mrb_value self)
{
  const char *name;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "z", &name);
  return mrb_bool_value(nk_window_is_collapsed(&(ctx->nk), name));
}

static mrb_value
mrb_gui_window_closedQ(mrb_state *mrb, mrb_value self)
{
  const char *name;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "z", &name);
  return mrb_bool_value(nk_window_is_closed(&(ctx->nk), name));
}

static mrb_value
mrb_gui_window_hiddenQ(mrb_state *mrb, mrb_value self)
{
  const char *name;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "z", &name);
  return mrb_bool_value(nk_window_is_hidden(&(ctx->nk), name));
}

static mrb_value
mrb_gui_window_activeQ(mrb_state *mrb, mrb_value self)
{
  const char *name;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "z", &name);
  return mrb_bool_value(nk_window_is_active(&(ctx->nk), name));
}

static mrb_value
mrb_gui_window_hoveredQ(mrb_state *mrb, mrb_value self)
{
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  return mrb_bool_value(nk_window_is_hovered(&(ctx->nk)));
}

static mrb_value
mrb_gui_window_set_focus(mrb_state *mrb, mrb_value self)
{
  const char *name;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "z", &name);
  nk_window_set_focus(&(ctx->nk), name);
  return self;
}

static mrb_value
mrb_gui_window_scroll(mrb_state *mrb, mrb_value self)
{
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  nk_uint x, y;
  nk_window_get_scroll(&(ctx->nk), &x, &y);
  return mrb_carbuncle_point_new(mrb, x, y);
}

static mrb_value
mrb_gui_window_set_scroll(mrb_state *mrb, mrb_value self)
{
  mrb_value point;
  Vector2 *data;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "o", &point);
  data = mrb_carbuncle_get_point(mrb, point);
  nk_window_set_scroll(&(ctx->nk), data->x, data->y);
  return self;
}

static mrb_value
mrb_gui_window_close(mrb_state *mrb, mrb_value self)
{
  const char *name;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "z", &name);
  nk_window_close(&(ctx->nk), name);
  return self;
}

static mrb_value
mrb_gui_window_minimize(mrb_state *mrb, mrb_value self)
{
  const char *name;
  mrb_bool minimized;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "zb", &name, &minimized);
  nk_window_collapse(&(ctx->nk), name, minimized ? NK_MINIMIZED : NK_MAXIMIZED);
  return self;
}

static mrb_value
mrb_gui_window_show(mrb_state *mrb, mrb_value self)
{
  const char *name;
  mrb_bool show;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_get_args(mrb, "zb", &name, &show);
  nk_window_show(&(ctx->nk), name, show ? NK_SHOWN : NK_HIDDEN);
  return self;
}

void
mrb_init_carbuncle_gui_window(mrb_state *mrb, struct RClass *gui)
{
  mrb_define_method(mrb, gui, "window", mrb_gui_window, MRB_ARGS_REQ(2)|MRB_ARGS_BLOCK()|MRB_ARGS_KEY(WINDOW_KEYS, 0));

  mrb_define_method(mrb, gui, "window_bounds", mrb_gui_window_bounds, MRB_ARGS_NONE());
  mrb_define_method(mrb, gui, "set_window_bounds", mrb_gui_window_set_bounds, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, gui, "window_focus?", mrb_gui_window_focusQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, gui, "window_focus", mrb_gui_window_focusQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, gui, "window_focus=", mrb_gui_window_set_focus, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, gui, "window_minimized?", mrb_gui_window_minimizedQ, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, gui, "window_closed?", mrb_gui_window_closedQ, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, gui, "window_hidden?", mrb_gui_window_hiddenQ, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, gui, "window_active?", mrb_gui_window_activeQ, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, gui, "window_hovered?", mrb_gui_window_hoveredQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, gui, "window_scroll", mrb_gui_window_scroll, MRB_ARGS_NONE());
  mrb_define_method(mrb, gui, "window_scroll=", mrb_gui_window_set_scroll, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, gui, "close_window", mrb_gui_window_close, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, gui, "minimize_window", mrb_gui_window_minimize, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, gui, "show_window", mrb_gui_window_show, MRB_ARGS_REQ(2));
}
