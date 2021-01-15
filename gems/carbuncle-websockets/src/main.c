#include "carbuncle/core.h"
#include "carbuncle/client.h"
#include "carbuncle/server.h"

void
mrb_carbuncle_websockets_gem_init(mrb_state *mrb)
{
  mrb_init_carbuncle_client(mrb);
  mrb_init_carbuncle_server(mrb);
}

void
mrb_carbuncle_websockets_gem_final(mrb_state *mrb)
{
  /* finalizer */
}
