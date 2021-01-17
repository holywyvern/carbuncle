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

struct mrb_Glyph
{
  FT_ULong codepoint;
  FT_BitmapGlyph bmp;
  Rectangle rect;
  Vector2 advance;
  Vector2 margin;
  struct mrb_Glyph *left;
  struct mrb_Glyph *right;
  mrb_int node_height;
  size_t row;
};

struct mrb_GlyphMap
{
  size_t count;
  struct mrb_Glyph *root;
};

struct mrb_FontMetrics
{
  mrb_int max_width;
  mrb_int min_height;
  mrb_int max_height;
};

struct mrb_Font
{
  FT_Face face;
  Font raylib_font;
  mrb_int size;
  struct mrb_GlyphMap glyphs;
  struct mrb_FontMetrics metrics;
  Texture2D texture;
  Image image;
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
