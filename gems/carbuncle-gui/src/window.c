#include "carbuncle/core.h"
#include "carbuncle/font.h"
#include "carbuncle/rect.h"

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

void
mrb_init_carbuncle_gui_window(mrb_state *mrb, struct RClass *gui)
{
  mrb_define_method(mrb, gui, "window", mrb_gui_window, MRB_ARGS_REQ(2)|MRB_ARGS_BLOCK()|MRB_ARGS_KEY(WINDOW_KEYS, 0));
}
