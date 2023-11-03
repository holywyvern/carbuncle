#include "carbuncle/core.h"
#include "carbuncle/container.h"

void
mrb_init_carbuncle_container(mrb_state *mrb)
{
  struct RClass *carbuncle = mrb_carbuncle_get(mrb);
  struct RClass *container = mrb_define_class_under(mrb, carbuncle, "Container", mrb->object_class);
}
