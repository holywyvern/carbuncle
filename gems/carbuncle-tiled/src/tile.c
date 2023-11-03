#include "carbuncle/core.h"
#include "carbuncle/point.h"
#include "carbuncle/color.h"
#include "carbuncle/tilemap.h"
#include "carbuncle/tilemap_tile.h"
#include "carbuncle/filesystem.h"

#include "raylib.h"

#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/array.h>
#include <mruby/variable.h>
#include <mruby/hash.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tmx_xml.h"

static mrb_value
mrb_tile_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value map;
  mrb_get_args(mrb, "o", &map);
  mrb_carbuncle_get_tilemap(mrb, map);
  mrb_iv_set(mrb, self, MAP_SYMBOL, map);
  return self;
}

static mrb_value
mrb_tile_get_map(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, MAP_SYMBOL);
}

static mrb_value
mrb_tile_get_id(mrb_state *mrb, mrb_value self)
{
  tmx_tile *tile = DATA_PTR(self);
  return mrb_fixnum_value(tile->id);
}

void
mrb_init_carbuncle_tilemap_tile(mrb_state *mrb, struct RClass *tilemap)
{
  struct RClass *tile = mrb_define_class_under(mrb, tilemap, "Tile", mrb->object_class);
  MRB_SET_INSTANCE_TT(tile, MRB_TT_DATA);

  mrb_define_method(mrb, tile, "initialize", mrb_tile_initialize, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, tile, "map", mrb_tile_get_map, MRB_ARGS_NONE());
  mrb_define_method(mrb, tile, "id", mrb_tile_get_id, MRB_ARGS_NONE());  
}
