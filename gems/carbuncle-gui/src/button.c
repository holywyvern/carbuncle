#include "carbuncle/core.h"
#include "carbuncle/font.h"
#include "carbuncle/rect.h"
#include "carbuncle/point.h"
#include "carbuncle/texture.h"

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
  nk_style_set_font(&(ctx->nk), &(ctx->font));
  result = nk_button_label(&(ctx->nk), title);
  return mrb_bool_value(result);
}

static mrb_value
gui_image_button(mrb_state *mrb, mrb_value self)
{
  Texture2D *tex;
  Rectangle *rec;
  mrb_value texture, rect;
  mrb_int align, argc;
  nk_bool result;
  const char *title;
  struct nk_image img;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  align = NK_TEXT_ALIGN_CENTERED|NK_TEXT_ALIGN_MIDDLE;
  argc = mrb_get_args(mrb, "oo|zi", &texture, &rect, &title, &align);
  tex = mrb_carbuncle_get_texture(mrb, texture);
  rec = mrb_carbuncle_get_rect(mrb, rect);
  img.handle.ptr = tex;
  img.w = tex->width;
  img.h = tex->height;
  img.region[0] = rec->x;
  img.region[1] = rec->y;
  img.region[2] = rec->width;
  img.region[3] = rec->height;
  if (argc < 3)
  {
    result = nk_button_image(&(ctx->nk), img);
  }
  else
  {
    result = nk_button_image_label(&(ctx->nk), img, title, align);
  }
  
  return mrb_bool_value(result);
}

void
mrb_init_carbuncle_gui_button(mrb_state *mrb, struct RClass *gui)
{
  mrb_define_method(mrb, gui, "button", gui_button, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, gui, "image_button", gui_image_button, MRB_ARGS_REQ(1));
}
