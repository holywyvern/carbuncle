#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>

#include "carbuncle/core.h"
#include "carbuncle/tile.h"
#include "carbuncle/tileset.h"
#include "carbuncle/tile_layer.h"

#include "raylib.h"

void
mrb_init_carbuncle_tileset(mrb_state *mrb)
{
  struct RClass *carbuncle = mrb_carbuncle_get(mrb);
  struct RClass *tile = mrb_define_class_under(mrb, carbuncle, "Tileset", mrb->object_class);
  MRB_SET_INSTANCE_TT(tile, MRB_TT_DATA);
}