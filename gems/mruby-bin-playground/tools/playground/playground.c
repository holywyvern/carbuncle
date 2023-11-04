#include <mruby.h>

#include <mruby/array.h>
#include <mruby/compile.h>
#include <mruby/string.h>
#include <mruby/error.h>

#include "carbuncle/core.h"
#include "carbuncle/filesystem.h"
#include "carbuncle/message_box.h"

#include <stdlib.h>
#include <stdio.h>

#include <raylib.h>

#include <physfs.h>

#ifndef __EMSCRIPTEN__
#error This Program is meant only for the web.
#endif

#include <emscripten/emscripten.h>

const char *NO_CARBUNCLE_GAME_MSG =
    "No game was found. To add a game define your extension of the Carbuncle::Game class:\n"
    "\n"
    "  class Playground < Carbuncle::Game\n"
    "    # ... your game code ...\n"
    "  end\n"
    "\n"
    "after that, Carbuncle will pick it automatically.";

const char *MULTIPLE_GAME_MSG =
    "Multiple games where found.\n"
    "The current Carbuncle version only supports one game.\n";

static void
close_game(const char *msg)
{
  puts(msg);
}

static void
check_mruby_error(mrb_state *mrb)
{
  if (!mrb->exc) return;

  mrb_print_error(mrb);
  mrb_print_backtrace(mrb);
}


static void
choose_game(mrb_state *mrb)
{
  mrb_value game_class = mrb_obj_value(mrb_class_get_under(mrb, mrb_module_get(mrb, "Carbuncle"), "Game"));
  mrb_value games = mrb_funcall(mrb, game_class, "descendants", 0);
  mrb_int len = RARRAY_LEN(games);
  if (len <= 0)
  {
    close_game(NO_CARBUNCLE_GAME_MSG);
  }
  if (len == 1)
  {
    mrb_funcall(mrb, mrb_ary_entry(games, 0), "run", 0);
    check_mruby_error(mrb);
    return;
  }
  close_game(MULTIPLE_GAME_MSG);
}

static mrb_value
start_engine(mrb_state *mrb, mrb_value code)
{
  mrbc_context *ctx = mrbc_context_new(mrb);
  mrbc_filename(mrb, ctx, "Playground");
  mrb_load_string_cxt(mrb, RSTRING_CSTR(mrb, code), ctx);
  choose_game(mrb);
  mrbc_context_free(mrb, ctx);
  return code;
}

int
main(int argc, char **argv)
{
  printf("Initializing Playground with %d arguments...", argc);
  mrb_state *mrb = mrb_open();
  if (!PHYSFS_init(argv[0]))
  {
    PHYSFS_ErrorCode code = PHYSFS_getLastErrorCode();
    printf("Failed to initialize filesystem: %s.\n", PHYSFS_getErrorByCode(code));
    mrb_carbuncle_show_fatal("File system error", "Failed to initialize filesystem");
    return EXIT_FAILURE;
  }
  if (argc < 2)
  {
    mrb_carbuncle_show_fatal("Playground Error", "Failed to initialize playground");
    return EXIT_FAILURE;
  }
  mrb_bool error;
  mrb_value result = mrb_protect(mrb, start_engine, mrb_str_new_cstr(mrb, argv[1]), &error);
  if (error)
  {
    mrb_carbuncle_show_error(mrb, result);
  }
  mrb_close(mrb);
  PHYSFS_deinit();
  return EXIT_SUCCESS;
}
