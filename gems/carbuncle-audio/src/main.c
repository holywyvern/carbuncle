#include <mruby.h>

#include "carbuncle/audio.h"
#include "carbuncle/music.h"
#include "carbuncle/sound.h"

#include "raylib.h"

void
mrb_carbuncle_audio_gem_init(mrb_state *mrb)
{
  InitAudioDevice();
  mrb_init_carbuncle_audio(mrb);
  mrb_init_carbuncle_music(mrb);
  mrb_init_carbuncle_sound(mrb);
}

void
mrb_carbuncle_audio_gem_final(mrb_state *mrb)
{
  CloseAudioDevice();
}