/**
 * This file should be called main.c but because of a YARD bug, it requires to be called a.rb to
 * allow other classes to be documented.
 */

#include <mruby.h>

#include <stdlib.h>

#include "raylib.h"

#include "carbuncle/container.h"
#include "carbuncle/game.h"
#include "carbuncle/screen.h"
#include "carbuncle/filesystem.h"
#include "carbuncle/message_box.h"
#include "carbuncle/loader.h"

#ifdef CARBUNCLE_DEBUG
mrb_bool mrb_carbuncle_debug_drawing = FALSE;

static mrb_value
mrb_s_carbuncle_debug_drawQ(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(mrb_carbuncle_debug_drawing);
}

static mrb_value
mrb_s_carbuncle_toggle_debug_draw(mrb_state *mrb, mrb_value self)
{
  mrb_carbuncle_debug_drawing = !mrb_carbuncle_debug_drawing;
  return mrb_bool_value(mrb_carbuncle_debug_drawing);
}

#endif

void
mrb_carbuncle_core_gem_init(mrb_state *mrb)
{
  mrb_define_module(mrb, "Carbuncle");

  mrb_init_carbuncle_container(mrb);
  mrb_init_carbuncle_game(mrb);
  mrb_init_carbuncle_screen(mrb);
  mrb_init_carbuncle_filesystem(mrb);
  mrb_init_carbuncle_messagebox(mrb);
  mrb_init_carbuncle_loader(mrb);

#ifdef CARBUNCLE_DEBUG
  mrb_define_module_function(mrb, carbuncle, "debug_draw?", mrb_s_carbuncle_debug_drawQ, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, carbuncle, "toggle_debug_draw", mrb_s_carbuncle_toggle_debug_draw, MRB_ARGS_NONE());
#endif
}

void
mrb_carbuncle_core_gem_final(mrb_state *mrb)
{
}
