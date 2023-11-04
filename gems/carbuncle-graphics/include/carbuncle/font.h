#ifndef CARBUNCLE_FONT_H
#define CARBUNCLE_FONT_H

#include <mruby.h>
#include "raylib.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#ifdef __cplusplus
extern "C" {
#endif

struct mrb_Font
{
  FT_Face face;
  Font raylib_font;
  mrb_float spacing;
  mrb_int size;
  mrb_bool is_default;
  char *bytes;
};


void
mrb_init_carbuncle_font(mrb_state *mrb);

struct mrb_Font *
mrb_carbuncle_get_font(mrb_state *mrb, mrb_value obj);

mrb_bool
mrb_carbuncle_font_p(mrb_value obj);

Vector2
mrb_carbuncle_font_measure_text(struct mrb_Font *font, const char *text);


#ifdef __cplusplus
}
#endif

#endif
