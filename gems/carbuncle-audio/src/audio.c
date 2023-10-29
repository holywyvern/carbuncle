#include "carbuncle/core.h"
#include "carbuncle/audio.h"
#include "carbuncle/filesystem.h"

static mrb_float master_volume = 100;

static mrb_value
mrb_s_audio_get_master_volume(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, master_volume);
}

static mrb_value
mrb_s_audio_set_master_volume(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  master_volume = value > 100 ? 100 : (value < 0 ? 0 : value);
  SetMasterVolume(master_volume / 100);
  return mrb_float_value(mrb, master_volume);
}

void
mrb_init_carbuncle_audio(mrb_state *mrb)
{
  struct RClass *carbuncle = mrb_carbuncle_get(mrb);
  /**
   * This class handles all global settings about audio.
   */
  struct RClass *audio = mrb_define_module_under(mrb, carbuncle, "Audio");

  mrb_define_module_function(mrb, audio, "master_volume", mrb_s_audio_get_master_volume, MRB_ARGS_NONE());

  mrb_define_module_function(mrb, audio, "master_volume=", mrb_s_audio_set_master_volume, MRB_ARGS_REQ(1));
}
