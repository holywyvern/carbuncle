#include "carbuncle/core.h"
#include "carbuncle/game.h"
#include "carbuncle/screen.h"
#include "carbuncle/message_box.h"
#include "carbuncle/loader.h"

#include "raylib.h"

#include <mruby/variable.h>
#include <mruby/gc.h>
#include <mruby/string.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/error.h>

#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

/* Constants */

#define CANVAS_SELECTOR "#canvas"

#ifdef __EMSCRIPTEN__
  static struct
  {
    mrb_state *mrb;
    mrb_value game;
  } mrb_em_state;

  mrb_bool emscripten_carbuncle_size_changed;
#endif

static const char *CURRENT_GAME_GV_NAME = "#carbunce_current_game";
static const char *SCREEN_IV_NAME = "#screen";

#define CURRENT_GAME_SYMBOL mrb_intern_cstr(mrb, CURRENT_GAME_GV_NAME)
#define SCREEN_SYMBOL mrb_intern_cstr(mrb, SCREEN_IV_NAME)
#define CHILDREN_SYMBOL mrb_intern_cstr(mrb, "@children")

/* Variables */

static mrb_bool carbuncle_game_is_running = FALSE;

/* Helper functions */

static inline void
create_window(mrb_state *mrb, mrb_value instance)
{
  mrb_value screen_value = mrb_iv_get(mrb, instance, SCREEN_SYMBOL);
  Screen *screen = mrb_carbuncle_get_screen(mrb, screen_value);
  mrb_value title_value = mrb_funcall(mrb, screen_value, "title", 0);
  const char *title = mrb_str_to_cstr(mrb, title_value);
  InitWindow(screen->width, screen->height, title);
  SetTargetFPS(60);
  if (!IsWindowReady())
  {
    mrb_carbuncle_show_fatal("Graphics Error", "Failed to initialize graphics");
  }
  InitAudioDevice();
  if (!IsAudioDeviceReady())
  {
    mrb_carbuncle_show_fatal("Audio Error", "Failed to initialize audio");
  }
}

static inline void
draw_loading_screen(mrb_state *mrb)
{
  const char *text = "Loading...";
  BeginDrawing();
  ClearBackground(RAYWHITE);
  int x = (GetScreenWidth() - MeasureText(text, 48)) / 2;
  int y = (GetScreenHeight() - 24) / 2;
  DrawText(text, x, y, 48, BLACK);
  EndDrawing();
}

static inline void
await_loaded_files(mrb_state *mrb)
{
  while (mrb_carbuncle_loader_loading_p(mrb))
  {
    draw_loading_screen(mrb);
  }
}

#ifdef __EMSCRIPTEN__

static void
carbuncle_emscripten_await_loaded_files()
{
  draw_loading_screen(mrb_em_state.mrb);
}

#endif

static void
preload_game(mrb_state *mrb, mrb_value self, mrb_value instance)
{
  // SetTraceLogLevel(LOG_ERROR);
  mrb_funcall(mrb, instance, "before_run", 0);
  mrb_gv_set(mrb, CURRENT_GAME_SYMBOL, instance);
  mrb_gc_register(mrb, instance);
  mrb_funcall(mrb, instance, "load", 0);
  carbuncle_game_is_running = TRUE;
  create_window(mrb, instance);
#ifdef __EMSCRIPTEN__
  mrb_em_state.mrb = mrb;
  mrb_em_state.game = instance;
  emscripten_set_main_loop(carbuncle_emscripten_await_loaded_files, 0, 0);
  while (mrb_carbuncle_loader_loading_p(mrb))
  {
    emscripten_sleep(16);
  }
  emscripten_cancel_main_loop();
#else
  await_loaded_files(mrb);
#endif
}

static inline void
begin_game(mrb_state *mrb, mrb_value self, mrb_value instance)
{
  mrb_funcall(mrb, instance, "start", 0);
  mrb_funcall(mrb, instance, "after_run", 0);
#ifdef __EMSCRIPTEN__
  emscripten_carbuncle_size_changed = true;
#endif
}

static inline void
check_closing(mrb_state *mrb, mrb_value instance)
{
  if (WindowShouldClose())
  {
    mrb_value should_close = mrb_funcall(mrb, instance, "close?", 0);
    if (mrb_test(should_close))
    {
      mrb_funcall(mrb, instance, "close", 0);
    }
  }  
}

static inline void
draw_game(mrb_state *mrb, mrb_value instance)
{
#ifdef CARBUNCLE_DEBUG
  mrb_carbuncle_debug_color_index = 0;
#endif
  BeginDrawing();
  ClearBackground(BLACK);
  mrb_funcall(mrb, instance, "draw", 0);
  EndDrawing();
}

static inline void
update_file_drop(mrb_state *mrb, mrb_value instance)
{
  if (IsFileDropped())
  {
    mrb_int arena;
    FilePathList files = LoadDroppedFiles();
    arena = mrb_gc_arena_save(mrb);
    {
      mrb_value file_values[files.count];
      for (int i = 0; i < files.count; ++i)
      {
        file_values[i] = mrb_str_new_cstr(mrb, files.paths[i]);
      }        
      mrb_funcall_argv(mrb, instance, mrb_intern_cstr(mrb, "file_dropped"), files.count, file_values);
    }
    mrb_gc_arena_restore(mrb, arena);
    UnloadDroppedFiles(files);
  }
}

static void
game_frame(mrb_state *mrb, mrb_value game)
{
#ifdef __EMSCRIPTEN__
  if (emscripten_carbuncle_size_changed)
  {
    double w, h;
    emscripten_get_element_css_size(CANVAS_SELECTOR, &w, &h);    
    mrb_funcall(mrb, game, "on_resize", 2, mrb_float_value(mrb, w), mrb_float_value(mrb, h));
    emscripten_carbuncle_size_changed = FALSE;
  }
#endif  
  mrb_value dt = mrb_float_value(mrb, GetFrameTime());
  check_closing(mrb, game);
  update_file_drop(mrb, game);
  struct RClass *audio = mrb_carbuncle_module_get(mrb, "Audio");
  mrb_funcall(mrb, mrb_obj_value(audio), "update", 1, dt);
  mrb_funcall(mrb, game, "update", 1, dt);
  draw_game(mrb, game);
}

#ifdef __EMSCRIPTEN__

void
mrb_carbuncle_audio_gem_final(mrb_state *mrb);

static mrb_value
secure_loop(mrb_state *mrb, mrb_value self)
{
  game_frame(mrb, mrb_em_state.game);
  return self;
}

static void
carbuncle_emscripten_game_frame()
{
  mrb_bool error;
  mrb_value result = mrb_protect(mrb_em_state.mrb, secure_loop, mrb_nil_value(), &error);
  if (error)
  {
    mrb_carbuncle_show_error(mrb_em_state.mrb, result);
    emscripten_cancel_main_loop();
    mrb_carbuncle_audio_gem_final(mrb_em_state.mrb);
  }
  if (!carbuncle_game_is_running)
  {
    emscripten_cancel_main_loop();
    mrb_carbuncle_audio_gem_final(mrb_em_state.mrb);
  }
}
#endif

static inline void
game_loop(mrb_state *mrb, mrb_value instance)
{
#ifdef __EMSCRIPTEN__
  mrb_em_state.mrb = mrb;
  mrb_em_state.game = instance;
  emscripten_set_main_loop(carbuncle_emscripten_game_frame, 0, 1);
#else
  while (carbuncle_game_is_running)
  {
    game_frame(mrb, instance);
  }
#endif
}

static inline void
close_game(mrb_state *mrb, mrb_value self, mrb_value instance)
{
  mrb_gc_unregister(mrb, instance);
  mrb_gv_set(mrb, CURRENT_GAME_SYMBOL, mrb_nil_value());
  mrb_funcall(mrb, instance, "before_close", 0);
  CloseAudioDevice();
  CloseWindow();
  mrb_funcall(mrb, instance, "after_close", 0);
}

#ifdef __EMSCRIPTEN__

static EM_BOOL
on_web_display_size_changed(int event_type, const EmscriptenUiEvent *event, void *user_data)
{
  emscripten_carbuncle_size_changed = TRUE;
  return FALSE;
}

#endif

static inline void
add_listeners(mrb_state *mrb, mrb_value self, mrb_value instance)
{
#ifdef __EMSCRIPTEN__
  mrb_em_state.mrb = mrb;
  mrb_em_state.game = instance;
  emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 0, on_web_display_size_changed);
#endif
}

/* Ruby Bindings */

/**
 * Runs an instance of this specific game class and initializes its main loop.
 * @raise [Carbuncle::GameIsRunning] if a game is already running.
 * @return [nil]
 */
static mrb_value
mrb_s_game_run(mrb_state *mrb, mrb_value self)
{
  mrb_value instance = mrb_funcall(mrb, self, "new", 0);
  preload_game(mrb, self, instance);
  begin_game(mrb, self, instance);
  add_listeners(mrb, self, instance);
  game_loop(mrb, instance);
  close_game(mrb, self, instance);
  return mrb_nil_value();
}

/**
 * Returns true if a game is already running
 * @return [Boolean]
 */
static mrb_value
mrb_s_game_runningQ(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(carbuncle_game_is_running);
}

/**
 * Returns the current game if its running.
 * @return [Carbuncle::Game, nil] the current game that's running or nil if no game is running.
 */
static mrb_value
mrb_s_game_get_current_game(mrb_state *mrb, mrb_value self)
{
  return mrb_gv_get(mrb, CURRENT_GAME_SYMBOL);
}

/**
 * @return [Carbuncle::Screen]
 */
static mrb_value
mrb_game_get_screen(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, SCREEN_SYMBOL);
}

/**
 * @return [self]
 */
static mrb_value
mrb_game_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value new_screen = mrb_carbuncle_screen_new(mrb, self);
  mrb_iv_set(mrb, self, SCREEN_SYMBOL, new_screen);
  mrb_iv_set(mrb, self, CHILDREN_SYMBOL, mrb_ary_new(mrb));
  return self;
}

/**
 * An method called when the user clicks on the cross button.
 * It should return true/false depending if the user can and can't close the screen.
 * Useful to redirect your close button to another action.
 * @return [self]
 */
static mrb_value
mrb_game_closeQ(mrb_state *mrb, mrb_value self)
{
  return mrb_true_value();
}

/**
 * Closes the game and destroys the game's window.
 * @return [self]
 */
static mrb_value
mrb_game_close(mrb_state *mrb, mrb_value self)
{
  carbuncle_game_is_running = FALSE;
  return mrb_nil_value();
}

/**
 * @overload resize(width, height)
 *   Resizes the game's window.
 *   @param [Integer] width The new screen's width.
 *   @param [Integer] height The new screen's height.
 *   @return [nil]
 */
static mrb_value
mrb_game_resize(mrb_state *mrb, mrb_value self)
{
  mrb_int width, height;
  mrb_get_args(mrb, "ii", &width, &height);
  SetWindowSize(width, height);
#ifdef __EMSCRIPTEN__
  emscripten_set_canvas_element_size(CANVAS_SELECTOR, width, height);
#endif
  return mrb_nil_value();
}

/* Init Game class */

void
mrb_init_carbuncle_game(mrb_state *mrb)
{
  struct RClass *carbuncle = mrb_carbuncle_get(mrb);
  /**
   * This class represents a game. It handles screen creation and the game's main loop.
   *
   * @!attribute [r] screen
   *   The screen attached to this game.
   */
  struct RClass *game = mrb_define_class_under(mrb, carbuncle, "Game", mrb_class_get_under(mrb, carbuncle, "Container"));

  /* Class methods */

  mrb_define_class_method(mrb, game, "run", mrb_s_game_run, MRB_ARGS_NONE());

  mrb_define_class_method(mrb, game, "running?", mrb_s_game_runningQ, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, game, "current_game", mrb_s_game_get_current_game, MRB_ARGS_NONE());

  /* Getters */

  mrb_define_method(mrb, game, "screen", mrb_game_get_screen, MRB_ARGS_NONE());

  /* Instance Methods */
  mrb_define_method(mrb, game, "initialize", mrb_game_initialize, MRB_ARGS_NONE());
  mrb_define_method(mrb, game, "close?", mrb_game_closeQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, game, "resize", mrb_game_resize, MRB_ARGS_REQ(2));

  /* Game lifecycle */
  mrb_define_method(mrb, game, "close", mrb_game_close, MRB_ARGS_NONE());

  mrb_gv_set(mrb, CURRENT_GAME_SYMBOL, mrb_nil_value());
}

mrb_value
mrb_carbuncle_get_current_game(mrb_state *mrb)
{
  return mrb_gv_get(mrb, CURRENT_GAME_SYMBOL);
}

mrb_bool
mrb_carbuncle_is_current_game(mrb_state *mrb, mrb_value self)
{
  mrb_value current_game = mrb_carbuncle_get_current_game(mrb);
  return mrb_equal(mrb, self, current_game);
}