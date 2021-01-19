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

#define LIST_VIEW_KEYS 13

static const char *LIST_VIEW_KEYWORDS[LIST_VIEW_KEYS] = {
  "border", "movable", "scalable", "closable", "minimizable",
  "disable_scrollbar", "title", "auto_scrollbar", "background", "scale_left", "disable",
  "minimized", "maximized"  
};

static mrb_value
gui_list_view(mrb_state *mrb, mrb_value self)
{
  mrb_value kw_values[LIST_VIEW_KEYS];
  mrb_value block;
  const char *id;
  mrb_int height, rows;
  struct nk_list_view view;
  nk_flags flags = 0;
  const mrb_kwargs kwargs = { LIST_VIEW_KEYS, kw_values, LIST_VIEW_KEYWORDS, 0, NULL };
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self); 
  mrb_get_args(mrb, "zii:&", &id, &height, &rows, &kwargs, &block);
  if (mrb_carbuncle_test(kw_values[0]))  { flags |= NK_WINDOW_BORDER; }
  if (mrb_carbuncle_test(kw_values[1]))  { flags |= NK_WINDOW_MOVABLE; }
  if (mrb_carbuncle_test(kw_values[2]))  { flags |= NK_WINDOW_SCALABLE; }
  if (mrb_carbuncle_test(kw_values[3]))  { flags |= NK_WINDOW_CLOSABLE; }
  if (mrb_carbuncle_test(kw_values[4]))  { flags |= NK_WINDOW_MINIMIZABLE; }
  if (mrb_carbuncle_test(kw_values[5]))  { flags |= NK_WINDOW_NO_SCROLLBAR; }
  if (mrb_carbuncle_test(kw_values[6]))  { flags |= NK_WINDOW_TITLE; }
  if (mrb_carbuncle_test(kw_values[7]))  { flags |= NK_WINDOW_SCROLL_AUTO_HIDE; }
  if (mrb_carbuncle_test(kw_values[8]))  { flags |= NK_WINDOW_BACKGROUND; }
  if (mrb_carbuncle_test(kw_values[9])) { flags |= NK_WINDOW_SCALE_LEFT; }
  if (mrb_carbuncle_test(kw_values[10])) { flags |= NK_WINDOW_NO_INPUT; }
  if (mrb_carbuncle_test(kw_values[11])) { flags |= NK_MINIMIZED; }
  if (mrb_carbuncle_test(kw_values[12])) { flags |= NK_MAXIMIZED; }
  if (nk_list_view_begin(&(ctx->nk), &view, id, flags, height, rows))
  {
    mrb_yield_argv(mrb, block, 0, NULL);
    nk_list_view_end(&(ctx->nk));
  }
  return self;
}

void
mrb_init_carbuncle_gui_list_view(mrb_state *mrb, struct RClass *gui)
{
  mrb_define_method(mrb, gui, "list_view", gui_list_view, MRB_ARGS_REQ(3)|MRB_ARGS_BLOCK()|MRB_ARGS_KEY(LIST_VIEW_KEYS, 0));
}
