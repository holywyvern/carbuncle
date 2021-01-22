#ifndef CARBUNCLE_NINE_PATCH_H
#define CARBUNCLE_NINE_PATCH_H

#include <mruby.h>

#include "raylib.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mrb_9Patch
{
  mrb_float  angle;
  mrb_float  width, height, left, top, right, bottom;
  Texture   *texture;
  Rectangle *src_rect;
  Vector2   *position;
  Vector2   *pivot;
  Color     *color;
};

void
mrb_init_carbuncle_nine_patch(mrb_state *mrb);

struct mrb_9Patch *
mrb_carbuncle_get_nine_patch(mrb_state *mrb, mrb_value obj);

mrb_bool
mrb_carbuncle_nine_patch_p(mrb_value obj);

#ifdef __cplusplus
}
#endif

#endif
