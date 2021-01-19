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

#define GROUP_KEYS 14

static const char *GROUP_KEYWORDS[GROUP_KEYS] = {
  "id", "border", "movable", "scalable", "closable", "minimizable",
  "disable_scrollbar", "title", "auto_scrollbar", "background", "scale_left", "disable",
  "minimized", "maximized"  
};

static mrb_value
gui_group(mrb_state *mrb, mrb_value self)
{
  const char *name, *title_str;
  nk_flags flags;
  mrb_value block, title;
  mrb_value kw_values[GROUP_KEYS];
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  const mrb_kwargs kwargs = { GROUP_KEYS, kw_values, GROUP_KEYWORDS, 0, NULL };
  mrb_get_args(mrb, "S:&", &title, &kwargs, &block);
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
  name = mrb_string_cstr(mrb, kw_values[0]);
  title_str = mrb_string_cstr(mrb, title);
  if (nk_group_begin_titled(&(ctx->nk), name, title_str, flags)) {
    mrb_yield_argv(mrb, block, 0, NULL);
    nk_group_end(&(ctx->nk));
  }
  return self;
}

void
mrb_init_carbuncle_gui_group(mrb_state *mrb, struct RClass *gui)
{
  mrb_define_method(mrb, gui, "group", gui_group, MRB_ARGS_REQ(1)|MRB_ARGS_BLOCK()|MRB_ARGS_KEY(GROUP_KEYS, 0));
}