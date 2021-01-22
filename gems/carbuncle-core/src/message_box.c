#include <mruby.h>
#include <mruby/string.h>
#include <mruby/array.h>

#include "carbuncle/message_box.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>

EM_JS(void, emscripten_alert, (const char* str), {
  alert(UTF8ToString(str));
})

#elif __unix__
  // TODO: Add proper error message
  #include <stdio.h>
#else
#include <boxer/boxer.h>
#endif

void
mrb_carbuncle_show_fatal(const char *title, const char *message)
{
#ifdef __EMSCRIPTEN__
    emscripten_alert(message);
#elif __unix__
    // TODO: Add proper error message
    puts(title);
    puts(message);
    getc(stdin);
#else
    boxerShow(message, title, BoxerStyleError, BoxerSelectionOK);
#endif
}

void
mrb_carbuncle_show_error(mrb_state *mrb, mrb_value error)
{
  mrb_value str = mrb_inspect(mrb, error);
  mrb_value newline = mrb_str_new_cstr(mrb, "\n");
  mrb_value backtrace = mrb_ary_join(mrb, mrb_funcall(mrb, error, "backtrace", 0), newline);
  str = mrb_str_append(mrb, str, mrb_str_new_cstr(mrb, " in:\n"));
  str = mrb_str_append(mrb, str, backtrace);
  mrb_carbuncle_show_fatal(mrb_obj_classname(mrb, error), mrb_str_to_cstr(mrb, str));
}

void
mrb_init_carbuncle_messagebox(mrb_state *mrb)
{
}
