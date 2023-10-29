#include "carbuncle/core.h"
#include "carbuncle/font.h"
#include "carbuncle/point.h"
#include "carbuncle/rect.h"

#include <mruby/data.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include <mruby/array.h>
#include <mruby/class.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "microutf8.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

Image
LoadImageEx(Color *pixels, int width, int height);

static void
mrb_font_free(mrb_state *mrb, void *ptr)
{
  struct mrb_Font *font = ptr;
  if (font)
  {
    if (!font->is_default)
    {
      UnloadFont(font->raylib_font);
    }
    FT_Done_Face(font->face);
    mrb_free(mrb, font->bytes);
    mrb_free(mrb, font);
  }
}

static const struct mrb_data_type font_data_type = {
  "Carbuncle::Font", mrb_font_free
};

static FT_Library carbuncle_freetype;

static inline void
load_glyphs(mrb_state *mrb, struct mrb_Font *font, FT_Open_Args *args)
{
  FT_UInt index;
  int *glyphs = mrb_alloca(mrb, sizeof(*glyphs) * font->face->num_glyphs);
  FT_UInt i = 0;
  FT_ULong codepoint = FT_Get_First_Char(font->face, &index);
  while (codepoint)
  {
    glyphs[i] = (int)codepoint;
    codepoint = FT_Get_Next_Char(font->face, codepoint, &index);
    ++i;
  }
  font->raylib_font = LoadFontFromMemory(
    GetFileExtension(args->pathname), args->memory_base, args->memory_size,
    font->size, glyphs, font->face->num_glyphs);
}

static inline void
open_font(mrb_state *mrb, struct mrb_Font *font, const char *filename, size_t size)
{
  FT_Error error;
  size_t bytes_size;
  char *bytes;
  FT_Open_Args args;
  bytes = mrb_carbuncle_load_file(mrb, filename, &bytes_size);
  args.flags = FT_OPEN_MEMORY;
  args.memory_base = bytes;
  args.memory_size = bytes_size;
  args.pathname = filename;
  error = FT_Open_Face(carbuncle_freetype, &args, 0, &(font->face));
  font->bytes = bytes;
  if (error)
  {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "cannot load font '%s'.", filename);
  }
  if (FT_Select_Charmap(font->face, FT_ENCODING_UNICODE))
  {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "cannot load font '%s' as unicode.", filename);
  }
  if (FT_Set_Pixel_Sizes(font->face, 0, size))
  {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "cannot set font size for font %s.", filename);
  }
  load_glyphs(mrb, font, &args);
  SetTextureFilter(font->raylib_font.texture, TEXTURE_FILTER_POINT);
}

static mrb_value
mrb_font_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int size, argc;
  const char *name = NULL;
  mrb_value font_class = mrb_obj_value(mrb_carbuncle_class_get(mrb, "Font"));
  argc = mrb_get_args(mrb, "|zi", &name, &size);
  if (argc < 1)
  {
    mrb_value default_name = mrb_funcall(mrb, font_class, "default_name", 0);
    if (!mrb_nil_p(default_name))
    {
      name = mrb_str_to_cstr(mrb, default_name);
    }
  }
  if (argc < 2)
  {
    size = mrb_fixnum(mrb_to_int(mrb, mrb_funcall(mrb, font_class, "default_size", 0)));
  }
  struct mrb_Font *font = mrb_malloc(mrb, sizeof *font);
  DATA_PTR(self) = font;
  DATA_TYPE(self) = &font_data_type;
  font->face = NULL;
  font->size = size;
  
  if (name)
  {
    mrb_carbuncle_check_file(mrb, name);
    open_font(mrb, font, name, size);
    font->is_default = FALSE;
  }
  else
  {
    font->raylib_font = GetFontDefault();
    font->is_default = TRUE;
  }
  return self;
}

static mrb_value
mrb_font_disposedQ(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(!DATA_PTR(self));
}

static mrb_value
mrb_font_dispose(mrb_state *mrb, mrb_value self)
{
  struct mrb_Font *font = mrb_carbuncle_get_font(mrb, self);
  mrb_font_free(mrb, font);
  DATA_PTR(self) = NULL;
  return self;
}

static mrb_value
mrb_font_measure_text(mrb_state *mrb, mrb_value self)
{
  const char *text;
  struct mrb_Font *font = mrb_carbuncle_get_font(mrb, self);
  mrb_get_args(mrb, "z", &text);
  Vector2 size = mrb_carbuncle_font_measure_text(font, text);
  return mrb_carbuncle_point_new(mrb, size.x, size.y);
}

static mrb_value
mrb_font_text_rect(mrb_state *mrb, mrb_value self)
{
  const char *text;
  struct mrb_Font *font = mrb_carbuncle_get_font(mrb, self);
  mrb_get_args(mrb, "z", &text);
  Vector2 size = mrb_carbuncle_font_measure_text(font, text);
  return mrb_carbuncle_rect_new(mrb, 0, 0, size.x, size.y);
}

void
mrb_init_carbuncle_font(mrb_state *mrb)
{
  if (FT_Init_FreeType( &carbuncle_freetype ))
  {
    mrb_raise(mrb, mrb->eStandardError_class, "Unable to initialize freetype.");
  }
  struct RClass *carbuncle = mrb_carbuncle_get(mrb);
  struct RClass *font = mrb_define_class_under(mrb, carbuncle, "Font", mrb->object_class);
  MRB_SET_INSTANCE_TT(font, MRB_TT_DATA);

  mrb_define_method(mrb, font, "initialize", mrb_font_initialize, MRB_ARGS_OPT(2));

  mrb_define_method(mrb, font, "disposed?", mrb_font_disposedQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, font, "dispose", mrb_font_dispose, MRB_ARGS_NONE());

  mrb_define_method(mrb, font, "measure_text", mrb_font_measure_text, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, font, "text_rect", mrb_font_text_rect, MRB_ARGS_REQ(1));
}

struct mrb_Font *
mrb_carbuncle_get_font(mrb_state *mrb, mrb_value obj)
{
  return DATA_GET_DISPOSABLE_PTR(mrb, obj, &font_data_type, struct mrb_Font);
}

mrb_bool
mrb_carbuncle_font_p(mrb_value obj)
{
  return mrb_data_p(obj) && (DATA_TYPE(obj) == &font_data_type);
}

Vector2
mrb_carbuncle_font_measure_text(struct mrb_Font *font, const char *msg)
{
  return MeasureTextEx(font->raylib_font, msg, font->size, 0);
}
