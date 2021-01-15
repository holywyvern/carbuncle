#include <mruby.h>

#include <mruby/array.h>
#include <mruby/compile.h>

#include "carbuncle/core.h"
#include "carbuncle/filesystem.h"

#include <stdlib.h>
#include <stdio.h>

#include <raylib.h>

#include <physfs.h>

const char *MAIN_FILENAME = "main.rb";

const char *NO_CARBUNCLE_GAME_MSG =
  "No game was found. To add a game define your extension of the Carbuncle::Game class:\n"
  "\n"
  "  class MyGame < Carbuncle::Game\n"
  "    # ... your game code ...\n"
  "  end\n"
  "\n"
  "after that, Carbuncle will pick it automatically."
;

const char *MULTIPLE_GAME_MSG =
  "Multiple games where found.\n"
  "The current Carbuncle version only supports one game.\n"
;

const char *NO_MAIN_FILE_MSG = "Cannot load file main.rb, it wasn't found.";

const char *CANNOT_CHANGE_DIRECTORY_MSG = "Cannot load file main.rb, it wasn't found.";

#define ERROR_MESSAGE PHYSFS_getErrorByCode(code)

static void
wait_for_close()
{
  char key[1];
  puts("Press any key to continue...");
  fgets(key, 1, stdin);  
  exit(EXIT_FAILURE);
}

static void
close_game(const char *msg)
{
  puts(msg);
  wait_for_close();
}

static void
check_mruby_error(mrb_state *mrb)
{
  if (mrb->exc)
  {
    mrb_print_error(mrb);
    mrb_print_backtrace(mrb);
    wait_for_close();
  }
}

static void
set_working_directory(mrb_state *mrb, const char *file)
{
  const char *cwd = GetPrevDirectoryPath(file);
  if (!PHYSFS_mount(cwd, NULL, 0))
  {
    close_game(CANNOT_CHANGE_DIRECTORY_MSG);
  }
  if (!ChangeDirectory(cwd))
  {
    close_game(CANNOT_CHANGE_DIRECTORY_MSG);
  }
}

static void
load_main_file(mrb_state *mrb)
{
  PHYSFS_sint64 length;
  const char *str;
  struct mrbc_context *ctx;
  mrb_value result;
  str = mrb_carbuncle_load_file(mrb, MAIN_FILENAME, &length);
  ctx = mrbc_context_new(mrb);
  ctx->filename = (char *)MAIN_FILENAME;
  ctx->capture_errors = TRUE;
  result = mrb_load_nstring_cxt(mrb, str, length, ctx);
  if (mrb->exc)
  {
    mrb_exc_raise(mrb, mrb_obj_value(mrb->exc));
  }
  mrb_free(mrb, str);
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

int
main(int argc, char **argv)
{
  mrb_state *mrb = mrb_open();
  if (!PHYSFS_init(argv[0]))
  {
    PHYSFS_ErrorCode code = PHYSFS_getLastErrorCode();
    printf("Failed to initialize filesystem: %s.\n", PHYSFS_getErrorByCode(code));
    return EXIT_FAILURE;
  }
  if (argv[0]) {
    set_working_directory(mrb, argv[0]);
    PHYSFS_mount(argv[0], NULL, 0); /* Check if itself is a zip file */
  }
  if (argc > 1)
  {
    if (!PHYSFS_mount(argv[argc - 1], NULL, 0))
    {
      PHYSFS_ErrorCode code = PHYSFS_getLastErrorCode();
      printf("Failed to mount filesystem for '%s' (%s).\n", argv[argc - 1], PHYSFS_getErrorByCode(code));
      return EXIT_FAILURE;
    }
  }
  load_main_file(mrb);
  choose_game(mrb);
  mrb_close(mrb);
  PHYSFS_deinit();
  return EXIT_SUCCESS;
}
