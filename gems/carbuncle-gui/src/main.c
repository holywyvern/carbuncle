#include "carbuncle/core.h"
#include "carbuncle/font.h"
#include "carbuncle/rect.h"

#include "microutf8.h"

#include <carbuncle/nuklear_config.h>

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include <mruby/error.h>
#include <mruby/array.h>

#include <math.h>

#define FONT_SYMBOL mrb_intern_cstr(mrb, "#font")
#define STYLE_SYMBOL mrb_intern_cstr(mrb, "#style")
#define ON_INPUT_SYMBOL mrb_intern_cstr(mrb, "#on_input")

static Color
nk_color_convert(struct nk_color color)
{
  return (Color) {
    color.r, color.g, color.b, color.a
  };
}

static void
update_mouse(struct nk_context *ctx, float prev_x, float prev_y)
{
  float x = GetMouseX();
  float y = GetMouseY();
  float scroll = GetMouseWheelMove();
  nk_input_button(ctx, NK_BUTTON_LEFT, x, y, IsMouseButtonDown(MOUSE_LEFT_BUTTON));
  nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, IsMouseButtonDown(MOUSE_MIDDLE_BUTTON));
  nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, IsMouseButtonDown(MOUSE_RIGHT_BUTTON));
  nk_input_scroll(ctx, (struct nk_vec2){0, scroll });
  if (x != prev_x || y != prev_y)
  {
    nk_input_motion(ctx, x, y);
  }
}

static void
update_keyboard(struct nk_context *ctx)
{
  nk_rune character = GetCharPressed();
  while (character)
  {
    nk_input_unicode(ctx, character);
    character = GetCharPressed();
  }
}

static void
update_input(mrb_state *mrb, mrb_value self, struct nk_context *ctx, float prev_x, float prev_y)
{
  nk_input_begin(ctx);
  update_mouse(ctx, prev_x, prev_y);
  update_keyboard(ctx);
  {
    mrb_value inputs = mrb_iv_get(mrb, self, ON_INPUT_SYMBOL);
    mrb_int len = RARRAY_LEN(inputs);
    for (mrb_int i = 0; i < len; ++i)
    {
      mrb_value item = mrb_ary_entry(inputs, i);
      mrb_yield_argv(mrb, item, 1, &self);
    }
  }
  nk_input_end(ctx);
}

static void
draw_text(struct nk_command_text *txt)
{
  Color bg, fg;
  struct mrb_Font *font = txt->font->userdata.ptr; 
  Vector2 pos = (Vector2){txt->x, txt->y};
  bg = nk_color_convert(txt->background);
  fg = nk_color_convert(txt->foreground);
  DrawRectangle(txt->x, txt->y, txt->w, txt->h, bg);
  DrawTextEx(
    font->raylib_font, txt->string, pos, font->size, 0, fg);
}

static void
draw_command(struct mrb_GuiContext *ctx, struct nk_command *cmd)
{
  Color color;
  switch (cmd->type)
  {
    case NK_COMMAND_SCISSOR:
    {
      struct nk_command_scissor *scissor = (struct nk_command_scissor *)cmd;
      if (ctx->scissor)
      {
        EndScissorMode();
      }
      BeginScissorMode(scissor->x, scissor->y, scissor->w, scissor->h);
      ctx->scissor = TRUE;
      break;
    }
    case NK_COMMAND_LINE:
    {
      struct nk_command_line *line = (struct nk_command_line *)cmd;
      Vector2 start = (Vector2){ line->begin.x, line->begin.y };
      Vector2 end = (Vector2){ line->end.x, line->end.y };
      color = nk_color_convert(line->color);
      DrawLineEx(start, end, line->line_thickness, color);
      break;
    }
    case NK_COMMAND_CURVE:
    {
      struct nk_command_curve *c = (struct nk_command_curve *)cmd;
      color = nk_color_convert(c->color);
      DrawLineBezier((Vector2){c->begin.x, c->begin.y}, (Vector2){c->end.x, c->end.y}, c->line_thickness, color);
      break;
    }
    case NK_COMMAND_RECT:
    {
      struct nk_command_rect *r = (struct nk_command_rect *)cmd;
      Rectangle rect = (Rectangle){ r->x, r->y, r->w, r->h };
      color = nk_color_convert(r->color);
      DrawRectangleRoundedLines(rect, r->rounding, r->rounding * 2, r->line_thickness, color);
      break;
    }
    case NK_COMMAND_RECT_FILLED:
    {
      struct nk_command_rect_filled *r = (struct nk_command_rect_filled *)cmd;
      Rectangle rect = (Rectangle){ r->x, r->y, r->w, r->h };
      color = nk_color_convert(r->color);
      DrawRectangleRounded(rect, r->rounding, r->rounding * 2, color);
      break;
    }
    case NK_COMMAND_RECT_MULTI_COLOR:
    {
      struct nk_command_rect_multi_color *r = (struct nk_command_rect_multi_color *)cmd;
      Rectangle rect = (Rectangle){ r->x, r->y, r->w, r->h };
      Color tl = nk_color_convert(r->left);
      Color tr = nk_color_convert(r->top);
      Color bl = nk_color_convert(r->right);
      Color br = nk_color_convert(r->bottom);
      DrawRectangleGradientEx(rect, tl, tr, bl, br);
      break;
    }
    case NK_COMMAND_CIRCLE:
    {
      struct nk_command_circle *circle = (struct nk_command_circle *)cmd;
      float r = fmin(circle->w, circle->h);
      color = nk_color_convert(circle->color);
      DrawCircleLines(circle->x, circle->y, r, color);
      break;
    }
    case NK_COMMAND_CIRCLE_FILLED:
    {
      struct nk_command_circle_filled *circle = (struct nk_command_circle_filled *)cmd;
      float r = fmin(circle->w, circle->h);
      color = nk_color_convert(circle->color);
      DrawCircle(circle->x, circle->y, r, color);
      break;
    }
    case NK_COMMAND_ARC:
    {
      struct nk_command_arc *arc = (struct nk_command_arc *)cmd;
      color = nk_color_convert(arc->color);
      DrawCircleSectorLines((Vector2){arc->cx, arc->cy}, arc->r, arc->a[0], arc->a[1], 100, color);
      break;
    }
    case NK_COMMAND_ARC_FILLED:
    {
      struct nk_command_arc_filled *arc = (struct nk_command_arc_filled *)cmd;
      DrawCircleSector((Vector2){arc->cx, arc->cy}, arc->r, arc->a[0], arc->a[1], 100, color);
      break;
    }
    case NK_COMMAND_TRIANGLE:
    {
      struct nk_command_triangle *triangle = (struct nk_command_triangle *)cmd;
      Vector2 v1 = (Vector2){ triangle->a.x, triangle->a.y };
      Vector2 v2 = (Vector2){ triangle->b.x, triangle->b.y };
      Vector2 v3 = (Vector2){ triangle->c.x, triangle->c.y };
      color = nk_color_convert(triangle->color);
      DrawTriangleLines(v1, v2, v3, color);
      break;
    }
    case NK_COMMAND_TRIANGLE_FILLED:
    {
      struct nk_command_triangle_filled *triangle = (struct nk_command_triangle_filled *)cmd;
      Vector2 v1 = (Vector2){ triangle->a.x, triangle->a.y };
      Vector2 v2 = (Vector2){ triangle->b.x, triangle->b.y };
      Vector2 v3 = (Vector2){ triangle->c.x, triangle->c.y };
      color = nk_color_convert(triangle->color);
      DrawTriangle(v1, v2, v3, color);      
      break;
    }
    case NK_COMMAND_POLYGON:
    {
      struct nk_command_polygon *poly = (struct nk_command_polygon *)cmd;
      color = nk_color_convert(poly->color);
      for (size_t i = 1; i < poly->point_count; ++i)
      {
        Vector2 start = (Vector2){ poly->points[i - 1].x, poly->points[i - 1].y };
        Vector2 end = (Vector2){ poly->points[i].x, poly->points[i].y };
        DrawLineEx(start, end, poly->line_thickness, color);
      }
      {
        Vector2 start = (Vector2){ poly->points[0].x, poly->points[0].y };
        Vector2 end = (Vector2){ poly->points[poly->point_count - 1].x, poly->points[poly->point_count - 1].y };
        DrawLineEx(start, end, poly->line_thickness, color);
      }
      break;
    }
    case NK_COMMAND_POLYGON_FILLED:
    {
      struct nk_command_polygon_filled *poly = (struct nk_command_polygon_filled *)cmd;
      color = nk_color_convert(poly->color);
      for (size_t i = 1; i < poly->point_count; ++i)
      {
        Vector2 start = (Vector2){ poly->points[i - 1].x, poly->points[i - 1].y };
        Vector2 end = (Vector2){ poly->points[i].x, poly->points[i].y };
        DrawLineEx(start, end, 1, color);
      }
      {
        Vector2 start = (Vector2){ poly->points[0].x, poly->points[0].y };
        Vector2 end = (Vector2){ poly->points[poly->point_count - 1].x, poly->points[poly->point_count - 1].y };
        DrawLineEx(start, end, 1, color);
      }      
      break;
    }
    case NK_COMMAND_TEXT:
    {
      struct nk_command_text *txt = (struct nk_command_text *)cmd;
      draw_text(txt);
      break;
    }
    case NK_COMMAND_IMAGE:
    {
      struct nk_command_image *img = (struct nk_command_image *)cmd;
      Rectangle src = (Rectangle){ img->img.region[0], img->img.region[1], img->img.region[0], img->img.region[0] };
      Rectangle dst = (Rectangle){ img->x, img->y, img->w, img->h };
      Texture2D texture = *(Texture2D *)img->img.handle.ptr;
      color = nk_color_convert(img->col);
      DrawTexturePro(texture, src, dst, (Vector2){0, 0}, 0, color);
      break;
    }
    default: { break; }
  }
}

static void
mrb_gui_free(mrb_state *mrb, void *ptr)
{
  if (ptr)
  {
    struct mrb_GuiContext *ctx = ptr;
    nk_free(&(ctx->nk));
    nk_end(&(ctx->nk));
    mrb_free(mrb, ctx);
  }
}


static const struct mrb_data_type gui_data_type = {
  "Carbuncle::GUI", mrb_gui_free
};

struct mrb_GuiContext *
mrb_carbuncle_gui_get_context(mrb_state *mrb, mrb_value self)
{
  return DATA_GET_DISPOSABLE_PTR(mrb, self, &gui_data_type, struct mrb_GuiContext);
}

static float
nk_carbuncle_font_width(nk_handle handle, float h, const char *text, int len)
{
  struct mrb_Font *fd = handle.ptr;
  return mrb_carbuncle_font_measure_text(fd, text).x;
}

static mrb_value
mrb_gui_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value font_value;
  struct mrb_GuiContext *ctx = mrb_malloc(mrb, sizeof *ctx);
  struct mrb_Font *fd;
  mrb_get_args(mrb, "o", &font_value);
  fd = mrb_carbuncle_get_font(mrb, font_value);
  ctx->font.userdata.ptr = fd;
  ctx->font.height = fd->size;
  ctx->font.width  = nk_carbuncle_font_width;
  mrb_iv_set(mrb, self, FONT_SYMBOL, font_value);
  mrb_iv_set(mrb, self, ON_INPUT_SYMBOL, mrb_ary_new(mrb));
  DATA_PTR(self) = ctx;
  DATA_TYPE(self) = &gui_data_type;
  if (!nk_init_default(&(ctx->nk), &(ctx->font)))
  {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Failed to initialize GUI.");
  }
  {
    struct RClass *gui = mrb_obj_class(mrb, self);
    struct RClass *style_class = mrb_class_get_under(mrb, gui, "Style");
    mrb_value style = mrb_obj_new(mrb, style_class, 1, &self);
    mrb_iv_set(mrb, self, STYLE_SYMBOL, style);
  }
  return self;
}

static mrb_value
mrb_gui_disposedQ(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(!DATA_PTR(self));
}

static mrb_value
mrb_gui_dispose(mrb_state *mrb, mrb_value self)
{
  void *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  mrb_free_context(mrb, ctx);
  DATA_PTR(self) = NULL;
  return self;
}

static mrb_value
mrb_gui_update(mrb_state *mrb, mrb_value self)
{
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  update_input(mrb, self, &(ctx->nk), ctx->mouse.x, ctx->mouse.y);
  ctx->mouse = GetMousePosition();
  return self;
}

static mrb_value
mrb_gui_draw(mrb_state *mrb, mrb_value self)
{
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  struct nk_context *nk = &(ctx->nk);
  const struct nk_command *cmd = NULL;
  ctx->scissor = FALSE;
  nk_foreach(cmd, nk)
  {
    draw_command(ctx, cmd);
  }
  if (ctx->scissor)
  {
    EndScissorMode();
  }
  nk_clear(nk);
  return self;
}

static mrb_value
mrb_gui_font(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, FONT_SYMBOL);
}

static mrb_value
mrb_gui_style(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, STYLE_SYMBOL);
}

static mrb_value
mrb_gui_on_input(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, ON_INPUT_SYMBOL);
}

static mrb_value
mrb_gui_set_input(mrb_state *mrb, mrb_value self)
{
  mrb_int key;
  mrb_bool is_down;
  struct mrb_GuiContext *ctx = mrb_carbuncle_gui_get_context(mrb, self);
  struct nk_context *nk = &(ctx->nk);
  mrb_get_args(mrb, "ib", &key, &is_down);
  nk_input_key(nk, key, is_down);
  return self;
}

void
mrb_init_carbuncle_gui_style(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_window(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_layout(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_group(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_tree(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_property(mrb_state *mrb, struct RClass *gui);

/* Widgets */

void
mrb_init_carbuncle_gui_button(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_chart(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_checkbox(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_color_picker(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_combo(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_edit(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_image(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_label(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_list_view(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_menu(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_popup(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_progress(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_radio(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_select(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_slider(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_text(mrb_state *mrb, struct RClass *gui);

void
mrb_init_carbuncle_gui_tooltip(mrb_state *mrb, struct RClass *gui);

void
mrb_carbuncle_gui_gem_init(mrb_state *mrb)
{
  struct RClass *carbuncle = mrb_carbuncle_get(mrb);
  struct RClass *gui = mrb_define_class_under(mrb, carbuncle, "GUI", mrb->object_class);
  MRB_SET_INSTANCE_TT(gui, MRB_TT_DATA);

  mrb_define_method(mrb, gui, "initialize", mrb_gui_initialize, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, gui, "disposed?", mrb_gui_disposedQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, gui, "dispose", mrb_gui_dispose, MRB_ARGS_NONE());

  mrb_define_method(mrb, gui, "font", mrb_gui_font, MRB_ARGS_NONE());
  mrb_define_method(mrb, gui, "style", mrb_gui_style, MRB_ARGS_NONE());

  mrb_init_carbuncle_gui_style(mrb, gui);
  mrb_init_carbuncle_gui_window(mrb, gui);
  mrb_init_carbuncle_gui_layout(mrb, gui);
  mrb_init_carbuncle_gui_group(mrb, gui);
  mrb_init_carbuncle_gui_tree(mrb, gui);
  mrb_init_carbuncle_gui_property(mrb, gui);
  // Widgets
  mrb_init_carbuncle_gui_button(mrb, gui);
  mrb_init_carbuncle_gui_chart(mrb, gui);
  mrb_init_carbuncle_gui_checkbox(mrb, gui);
  mrb_init_carbuncle_gui_color_picker(mrb, gui);
  mrb_init_carbuncle_gui_combo(mrb, gui);
  mrb_init_carbuncle_gui_edit(mrb, gui);
  mrb_init_carbuncle_gui_image(mrb, gui);
  mrb_init_carbuncle_gui_label(mrb, gui);
  mrb_init_carbuncle_gui_list_view(mrb, gui);
  mrb_init_carbuncle_gui_menu(mrb, gui);
  mrb_init_carbuncle_gui_popup(mrb, gui);
  mrb_init_carbuncle_gui_progress(mrb, gui);
  mrb_init_carbuncle_gui_radio(mrb, gui);
  mrb_init_carbuncle_gui_select(mrb, gui);
  mrb_init_carbuncle_gui_slider(mrb, gui);
  mrb_init_carbuncle_gui_text(mrb, gui);
  mrb_init_carbuncle_gui_tooltip(mrb, gui);

  mrb_define_method(mrb, gui, "update", mrb_gui_update, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, gui, "set_input", mrb_gui_set_input, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, gui, "on_input", mrb_gui_on_input, MRB_ARGS_NONE());
  mrb_define_method(mrb, gui, "draw", mrb_gui_draw, MRB_ARGS_NONE());

  struct RClass *keys = mrb_define_module_under(mrb, gui, "Keys");
  mrb_define_const(mrb, keys, "NONE", mrb_fixnum_value(NK_KEY_NONE));
  mrb_define_const(mrb, keys, "SHIFT", mrb_fixnum_value(NK_KEY_SHIFT));
  mrb_define_const(mrb, keys, "CTRL", mrb_fixnum_value(NK_KEY_CTRL));
  mrb_define_const(mrb, keys, "DEL", mrb_fixnum_value(NK_KEY_DEL));
  mrb_define_const(mrb, keys, "DELETE", mrb_fixnum_value(NK_KEY_DEL));
  mrb_define_const(mrb, keys, "ENTER", mrb_fixnum_value(NK_KEY_ENTER));
  mrb_define_const(mrb, keys, "RETURN", mrb_fixnum_value(NK_KEY_ENTER));
  mrb_define_const(mrb, keys, "TAB", mrb_fixnum_value(NK_KEY_TAB));
  mrb_define_const(mrb, keys, "BACKSPACE", mrb_fixnum_value(NK_KEY_BACKSPACE));
  mrb_define_const(mrb, keys, "COPY", mrb_fixnum_value(NK_KEY_COPY));
  mrb_define_const(mrb, keys, "CUT", mrb_fixnum_value(NK_KEY_CUT));
  mrb_define_const(mrb, keys, "PASTE", mrb_fixnum_value(NK_KEY_PASTE));
  mrb_define_const(mrb, keys, "UP", mrb_fixnum_value(NK_KEY_UP));
  mrb_define_const(mrb, keys, "DOWN", mrb_fixnum_value(NK_KEY_DOWN));
  mrb_define_const(mrb, keys, "LEFT", mrb_fixnum_value(NK_KEY_LEFT));
  mrb_define_const(mrb, keys, "RIGHT", mrb_fixnum_value(NK_KEY_RIGHT));
  mrb_define_const(mrb, keys, "TEXT_INSERT_MODE", mrb_fixnum_value(NK_KEY_TEXT_INSERT_MODE));
  mrb_define_const(mrb, keys, "TEXT_REPLACE_MODE", mrb_fixnum_value(NK_KEY_TEXT_REPLACE_MODE));
  mrb_define_const(mrb, keys, "TEXT_RESET_MODE", mrb_fixnum_value(NK_KEY_TEXT_RESET_MODE));
  mrb_define_const(mrb, keys, "TEXT_LINE_START", mrb_fixnum_value(NK_KEY_TEXT_LINE_START));
  mrb_define_const(mrb, keys, "TEXT_LINE_END", mrb_fixnum_value(NK_KEY_TEXT_LINE_END));
  mrb_define_const(mrb, keys, "TEXT_START", mrb_fixnum_value(NK_KEY_TEXT_START));
  mrb_define_const(mrb, keys, "TEXT_END", mrb_fixnum_value(NK_KEY_TEXT_END));
  mrb_define_const(mrb, keys, "TEXT_UNDO", mrb_fixnum_value(NK_KEY_TEXT_UNDO));
  mrb_define_const(mrb, keys, "TEXT_REDO", mrb_fixnum_value(NK_KEY_TEXT_REDO));
  mrb_define_const(mrb, keys, "TEXT_SELECT_ALL", mrb_fixnum_value(NK_KEY_TEXT_SELECT_ALL));
  mrb_define_const(mrb, keys, "TEXT_WORD_LEFT", mrb_fixnum_value(NK_KEY_TEXT_WORD_LEFT));
  mrb_define_const(mrb, keys, "TEXT_WORD_RIGHT", mrb_fixnum_value(NK_KEY_TEXT_WORD_RIGHT));
  mrb_define_const(mrb, keys, "SCROLL_START", mrb_fixnum_value(NK_KEY_SCROLL_START));
  mrb_define_const(mrb, keys, "SCROLL_END", mrb_fixnum_value(NK_KEY_SCROLL_END));
  mrb_define_const(mrb, keys, "SCROLL_DOWN", mrb_fixnum_value(NK_KEY_SCROLL_DOWN));
  mrb_define_const(mrb, keys, "SCROLL_UP", mrb_fixnum_value(NK_KEY_SCROLL_UP));
  mrb_define_const(mrb, keys, "KEY_MAX", mrb_fixnum_value(NK_KEY_MAX));
}

void
mrb_carbuncle_gui_gem_final(mrb_state *mrb)
{
  /* finalizer */
}