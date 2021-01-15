#include "carbuncle/server.h"

#include <libwebsockets.h>
#include <physfs.h>

#include <mruby/data.h>
#include <mruby/class.h>

#if defined(_WIN32) || defined(WIN32)
#include <io.h>
#else
#include <fcntl.h>
#endif

#include "carbuncle/ws_files.h"

void
mrb_init_carbuncle_server(mrb_state *mrb)
{
  struct RClass *carbuncle = mrb_module_get(mrb, "Carbuncle");
  struct RClass *ws = mrb_define_class_under(mrb, carbuncle, "Server", mrb->object_class);
  MRB_SET_INSTANCE_TT(ws, MRB_TT_DATA);
}
