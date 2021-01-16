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

#define BUTTON_SYMBOL mrb_intern_cstr(mrb, "#button")
#define CHART_SYMBOL mrb_intern_cstr(mrb, "#chart")
#define CHECKBOX_SYMBOL mrb_intern_cstr(mrb, "#checkbox")
#define COMBO_SYMBOL mrb_intern_cstr(mrb, "#combo")
#define CONTEXTUAL_BUTTON_SYMBOL mrb_intern_cstr(mrb, "#contextual_button")
#define CURSOR_ACTIVE_SYMBOL mrb_intern_cstr(mrb, "#cursor_active")
#define CURSOR_LAST_SYMBOL mrb_intern_cstr(mrb, "#cursor_last")
#define CURSOR_VISIBLE_SYMBOL mrb_intern_cstr(mrb, "#cursor_visible")
#define CURSORS_SYMBOL mrb_intern_cstr(mrb, "#cursors")
#define EDIT_SYMBOL mrb_intern_cstr(mrb, "#edit")
#define MENU_BUTTON_SYMBOL mrb_intern_cstr(mrb, "#menu_button")
#define OPTION_SYMBOL mrb_intern_cstr(mrb, "#option")
#define PROGRESS_SYMBOL mrb_intern_cstr(mrb, "#progress")
#define PROPERTY_SYMBOL mrb_intern_cstr(mrb, "#property")
#define SCROLLH_SYMBOL mrb_intern_cstr(mrb, "#scrollh")
#define SCROLLV_SYMBOL mrb_intern_cstr(mrb, "#scrollv")
#define SELECTABLE_SYMBOL mrb_intern_cstr(mrb, "#selectable")
#define SLIDER_SYMBOL mrb_intern_cstr(mrb, "#slider")
#define TAB_SYMBOL mrb_intern_cstr(mrb, "#tab")
#define TEXT_SYMBOL mrb_intern_cstr(mrb, "#text")
#define WINDOW_SYMBOL mrb_intern_cstr(mrb, "#window")

void
mrb_style_free(mrb_state *mrb, void *ptr) {}

static const struct mrb_data_type style_data_type = {
  "Carbuncle::GUI", mrb_style_free
};

static struct nk_style *
get_style(mrb_state *mrb, mrb_value self)
{
  struct nk_style *data;
  Data_Get_Struct(mrb, self, &style_data_type, data);
  return data;
}

mrb_value
mrb_style_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value gui;
  struct mrb_GuiContext *ctx;
  mrb_get_args(mrb, "o", &gui);
  ctx = mrb_carbuncle_gui_get_context(mrb, gui);
  DATA_TYPE(self) = &style_data_type;
  DATA_PTR(self) = &(ctx->nk.style);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "#gui"), gui);
  return self;
}

mrb_value
mrb_style_get_button(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, BUTTON_SYMBOL);
}

mrb_value
mrb_style_get_chart(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, CHART_SYMBOL);
}

mrb_value
mrb_style_get_checkbox(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, CHECKBOX_SYMBOL);
}

mrb_value
mrb_style_get_combo(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, COMBO_SYMBOL);
}

mrb_value
mrb_style_get_contextual_button(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, CONTEXTUAL_BUTTON_SYMBOL);
}

mrb_value
mrb_style_get_cursor_active(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, CURSOR_ACTIVE_SYMBOL);
}

mrb_value
mrb_style_get_cursor_last(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, CURSOR_LAST_SYMBOL);
}

mrb_value
mrb_style_get_cursor_visible(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, CURSOR_VISIBLE_SYMBOL);
}

mrb_value
mrb_style_get_cursors(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, CURSORS_SYMBOL);
}

mrb_value
mrb_style_get_edit(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, EDIT_SYMBOL);
}

mrb_value
mrb_style_get_menu_button(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, MENU_BUTTON_SYMBOL);
}

mrb_value
mrb_style_get_option(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, OPTION_SYMBOL);
}

mrb_value
mrb_style_get_progress(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, PROGRESS_SYMBOL);
}

mrb_value
mrb_style_get_property(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, PROPERTY_SYMBOL);
}

mrb_value
mrb_style_get_scrollh(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, SCROLLH_SYMBOL);
}

mrb_value
mrb_style_get_scrollv(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, SCROLLV_SYMBOL);
}

mrb_value
mrb_style_get_selectable(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, SELECTABLE_SYMBOL);
}

mrb_value
mrb_style_get_slider(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, SLIDER_SYMBOL);
}

mrb_value
mrb_style_get_tab(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, TAB_SYMBOL);
}

mrb_value
mrb_style_get_text(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, TEXT_SYMBOL);
}

mrb_value
mrb_style_get_window(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, WINDOW_SYMBOL);
}

void
mrb_init_carbuncle_gui_style(mrb_state *mrb, struct RClass *gui)
{
  struct RClass *style = mrb_define_class_under(mrb, gui, "Style", mrb->object_class);
  MRB_SET_INSTANCE_TT(style, MRB_TT_DATA);

  mrb_define_method(mrb, style, "initialize", mrb_style_initialize, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, style, "button", mrb_style_get_button, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "chart", mrb_style_get_chart, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "checkbox", mrb_style_get_checkbox, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "combo", mrb_style_get_combo, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "contextual_button", mrb_style_get_contextual_button, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "cursor_active", mrb_style_get_cursor_active, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "cursor_last", mrb_style_get_cursor_last, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "cursor_visible", mrb_style_get_cursor_visible, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "cursors", mrb_style_get_cursors, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "edit", mrb_style_get_edit, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "menu_button", mrb_style_get_menu_button, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "option", mrb_style_get_option, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "progress", mrb_style_get_progress, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "property", mrb_style_get_property, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "scrollh", mrb_style_get_scrollh, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "scrollv", mrb_style_get_scrollv, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "selectable", mrb_style_get_selectable, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "slider", mrb_style_get_slider, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "tab", mrb_style_get_tab, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "text", mrb_style_get_text, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "window", mrb_style_get_window, MRB_ARGS_NONE());

  struct RClass *color = mrb_define_class_under(mrb, style, "Color", mrb->object_class);
  MRB_SET_INSTANCE_TT(color, MRB_TT_DATA);

  struct RClass *image = mrb_define_class_under(mrb, style, "Image", mrb->object_class);
  MRB_SET_INSTANCE_TT(image, MRB_TT_DATA);

  struct RClass *item = mrb_define_class_under(mrb, style, "Item", mrb->object_class);
  MRB_SET_INSTANCE_TT(item, MRB_TT_DATA);

  struct RClass *vec2 = mrb_define_class_under(mrb, style, "Vec2", mrb->object_class);
  MRB_SET_INSTANCE_TT(item, MRB_TT_DATA);

  struct RClass *button = mrb_define_class_under(mrb, style, "Button", mrb->object_class);
  MRB_SET_INSTANCE_TT(button, MRB_TT_DATA);

  struct RClass *toggle = mrb_define_class_under(mrb, style, "Toggle", mrb->object_class);
  MRB_SET_INSTANCE_TT(toggle, MRB_TT_DATA);

  struct RClass *selectable = mrb_define_class_under(mrb, style, "Selectable", mrb->object_class);
  MRB_SET_INSTANCE_TT(selectable, MRB_TT_DATA);

  struct RClass *slider = mrb_define_class_under(mrb, style, "Slider", mrb->object_class);
  MRB_SET_INSTANCE_TT(slider, MRB_TT_DATA);

  struct RClass *progress = mrb_define_class_under(mrb, style, "Progress", mrb->object_class);
  MRB_SET_INSTANCE_TT(progress, MRB_TT_DATA);

  struct RClass *scrollbar = mrb_define_class_under(mrb, style, "Scrollbar", mrb->object_class);
  MRB_SET_INSTANCE_TT(scrollbar, MRB_TT_DATA);

  struct RClass *edit = mrb_define_class_under(mrb, style, "Edit", mrb->object_class);
  MRB_SET_INSTANCE_TT(edit, MRB_TT_DATA);

  struct RClass *property = mrb_define_class_under(mrb, style, "Property", mrb->object_class);
  MRB_SET_INSTANCE_TT(property, MRB_TT_DATA);

  struct RClass *chart = mrb_define_class_under(mrb, style, "Chart", mrb->object_class);
  MRB_SET_INSTANCE_TT(chart, MRB_TT_DATA);

  struct RClass *combo = mrb_define_class_under(mrb, style, "Combo", mrb->object_class);
  MRB_SET_INSTANCE_TT(combo, MRB_TT_DATA);

  struct RClass *tab = mrb_define_class_under(mrb, style, "Tab", mrb->object_class);
  MRB_SET_INSTANCE_TT(tab, MRB_TT_DATA);

  struct RClass *window = mrb_define_class_under(mrb, style, "Window", mrb->object_class);
  MRB_SET_INSTANCE_TT(window, MRB_TT_DATA);

  struct RClass *window_header = mrb_define_class_under(mrb, window, "Header", mrb->object_class);
  MRB_SET_INSTANCE_TT(window_header, MRB_TT_DATA);
}
