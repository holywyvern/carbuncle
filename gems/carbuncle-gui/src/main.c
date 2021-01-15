#include "carbuncle/core.h"
#include "carbuncle/font.h"
#include "carbuncle/rect.h"

#include "microutf8.h"

#define NK_IMPLEMENTATION
#define NK_PRIVATE
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_COMMAND_USERDATA
#define NK_KEYSTATE_BASED_INPUT
#define NK_INCLUDE_FIXED_TYPES
#include "nuklear.h"

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include <mruby/error.h>
#include <mruby/array.h>

#include <math.h>

#define FONT_SYMBOL mrb_intern_cstr(mrb, "#font")

#define WINDOW_KEYS 14

static const char *WINDOW_KEYWORDS[WINDOW_KEYS] = {
  "id", "border", "movable", "scalable", "closable", "minimizable",
  "disable_scrollbar", "title", "auto_scrollbar", "background", "scale_left", "disable",
  "minimized", "maximized"
};

struct mrb_GuiContext
{
  struct nk_context   nk;
  struct nk_user_font font;
  Vector2 mouse;
  mrb_bool scissor;
};

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
  nk_input_button(ctx, NK_BUTTON_LEFT, x, y, IsMouseButtonDown(MOUSE_LEFT_BUTTON));
  nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, IsMouseButtonDown(MOUSE_MIDDLE_BUTTON));
  nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, IsMouseButtonDown(MOUSE_RIGHT_BUTTON));
  if (x != prev_x || y != prev_y)
  {
    nk_input_motion(ctx, x, y);
  }
}

static void
update_keyboard(struct nk_context *ctx)
{
  nk_rune character = GetKeyPressed();
  nk_input_unicode(ctx, character);
}

static void
update_input(struct nk_context *ctx, float prev_x, float prev_y)
{
  nk_input_begin(ctx);
  update_mouse(ctx, prev_x, prev_y);
  update_keyboard(ctx);
  nk_input_end(ctx);
}

static void
draw_text(struct nk_command_text *txt)
{
  uint32_t codepoint;
  struct mrb_Glyph *glyph, *prev;
  const char *message = txt->string;
  size_t len = txt->length;
  mrb_int diff_h;
  struct mrb_Font *font = txt->font->userdata.ptr;
  {
    Vector2 position = (Vector2) {txt->x, txt->y};
    mrb_int min_h = font->metrics.max_height;
    mrb_int max_h = font->metrics.min_height;
    Color color = nk_color_convert(txt->foreground);
    struct mrb_Glyph *glyphs[len];
    for (size_t i = 0; i < len; ++i)
    {
      message = utf8_decode(message, &codepoint);
      glyph = mrb_carbuncle_font_get_glyph(font, codepoint);
      glyphs[i] = glyph;
      if (glyph)
      {
        mrb_int h = font->metrics.max_height - glyph->margin.y;
        if (h < min_h) { min_h = h; }
        if (h > max_h) { max_h = h; }
      }
    }
    prev = NULL;
    diff_h = max_h - min_h;
    for (size_t i = 0; i < len; ++i)
    {
      glyph = glyphs[i];
      if (glyph)
      {
        FT_Vector kerning;
        if (prev)
        {
          FT_Get_Kerning(font->face, prev->codepoint, glyph->codepoint, FT_KERNING_DEFAULT, &kerning);
          kerning.x = (kerning.x / font->face->units_per_EM) >> 6;          
        }
        else { kerning.x = 0; }
        Vector2 pos = (Vector2){
          position.x + glyph->margin.x + kerning.x,
          position.y + diff_h - glyph->margin.y + min_h
        };
        DrawTextureRec(font->texture, glyph->rect, pos, color);
        position.x += glyph->advance.x;
        position.y += glyph->advance.y;
      }
      prev = glyph;
    }
  }
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
      RenderTexture2D render = LoadRenderTexture(txt->w, txt->h);
      color = nk_color_convert(txt->background);
      DrawRectangle(txt->x, txt->y, txt->w, txt->h, color);
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

static struct mrb_GuiContext *
get_context(mrb_state *mrb, mrb_value self)
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
  DATA_PTR(self) = ctx;
  DATA_TYPE(self) = &gui_data_type;
  if (!nk_init_default(&(ctx->nk), &(ctx->font)))
  {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Failed to initialize GUI.");
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
  void *ctx = get_context(mrb, self);
  mrb_free_context(mrb, ctx);
  DATA_PTR(self) = NULL;
  return self;
}

static mrb_value
mrb_gui_update(mrb_state *mrb, mrb_value self)
{
  struct mrb_GuiContext *ctx = get_context(mrb, self);
  update_input(&(ctx->nk), ctx->mouse.x, ctx->mouse.y);
  ctx->mouse = GetMousePosition();
  return self;
}

static mrb_value
mrb_gui_draw(mrb_state *mrb, mrb_value self)
{
  struct mrb_GuiContext *ctx = get_context(mrb, self);
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
  struct mrb_GuiContext *ctx = get_context(mrb, self);
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
mrb_carbuncle_gui_gem_init(mrb_state *mrb)
{
  struct RClass *carbuncle = mrb_carbuncle_get(mrb);
  struct RClass *gui = mrb_define_class_under(mrb, carbuncle, "GUI", mrb->object_class);
  MRB_SET_INSTANCE_TT(gui, MRB_TT_DATA);

  mrb_define_method(mrb, gui, "initialize", mrb_gui_initialize, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, gui, "disposed?", mrb_gui_disposedQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, gui, "dispose", mrb_gui_dispose, MRB_ARGS_NONE());

  mrb_define_method(mrb, gui, "window", mrb_gui_window, MRB_ARGS_REQ(2)|MRB_ARGS_BLOCK()|MRB_ARGS_KEY(WINDOW_KEYS, 0));

  mrb_define_method(mrb, gui, "update", mrb_gui_update, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, gui, "draw", mrb_gui_draw, MRB_ARGS_NONE());
}

void
mrb_carbuncle_gui_gem_final(mrb_state *mrb)
{
  /* finalizer */
}