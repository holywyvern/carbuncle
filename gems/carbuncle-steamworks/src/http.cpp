#include <mruby.h>

extern "C"
void mrb_init_carbuncle_steamworks_http(mrb_state *mrb, struct RClass *steam)
{
  struct RClass *apps = mrb_define_module_under(mrb, steam, "HTTP");
}
