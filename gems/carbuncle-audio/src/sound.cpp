#include "carbuncle/core.h"
#include "carbuncle/sound.h"

#include <mruby/class.h>
#include <mruby/data.h>

#include "soloud.h"
#include "soloud_wav.h"
#include "physfs_file.hpp"

extern SoLoud::Soloud carbuncle_soloud;

namespace
{
  struct SoundContainer
  {
    SoLoud::Wav wav;
    PhysFsFile file;
    mrb_bool playing;
    SoLoud::handle handle;
  };
}

static void
mrb_sound_free(mrb_state *mrb, void *p)
{
  if (p)
  {
    SoundContainer *sound = reinterpret_cast<SoundContainer *>(p);
    delete sound;
  }
}

static const struct mrb_data_type sound_data_type = {
  "Carbuncle::Sound", mrb_sound_free
};

static SoundContainer *
get_sound(mrb_state *mrb, mrb_value self)
{
  return DATA_GET_DISPOSABLE_PTR(mrb, self, &sound_data_type, SoundContainer);
}

/**
 * @overload initialize(filename)
 *   Creates a new sound object from a filename
 *   @param [String] filename The name of the audio file.
 *   @raise [Carbuncle::FileNotFound] If the file does not exists.
 *   @return [self]
 */
static mrb_value
mrb_sound_initialize(mrb_state *mrb, mrb_value self)
{
  const char *name;
  mrb_get_args(mrb, "z", &name);
  mrb_carbuncle_check_file(mrb, name);
  SoundContainer *container = new SoundContainer();
  DATA_PTR(self) = container;
  DATA_TYPE(self) = &sound_data_type;
  container->file.init(mrb, name);
  container->playing = FALSE;
  if (container->wav.loadFile(&container->file))
  {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Couldn't load sound stream.");
  }
  container->handle = carbuncle_soloud.play(container->wav, -1, 0, true);
  return self;
}

/**
 * @return [Float]
 */
static mrb_value
mrb_sound_get_size(mrb_state *mrb, mrb_value self)
{
  SoundContainer *sound = get_sound(mrb, self);
  return mrb_float_value(mrb, sound->wav.getLength());
}

/**
 * The sound's volume in the rangre from 0..1
 * @return [Float]
 */
static mrb_value
mrb_sound_get_volume(mrb_state *mrb, mrb_value self)
{
  SoundContainer *sound = get_sound(mrb, self);
  return mrb_float_value(mrb, carbuncle_soloud.getVolume(sound->handle));
}

static mrb_value
mrb_sound_get_pan(mrb_state *mrb, mrb_value self)
{
  SoundContainer *sound = get_sound(mrb, self);
  return mrb_float_value(mrb, carbuncle_soloud.getPan(sound->handle));
}

/**
 * @return [Float]
 */
static mrb_value
mrb_sound_set_volume(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  SoundContainer *sound = get_sound(mrb, self);
  carbuncle_soloud.setVolume(sound->handle, value);
  return mrb_float_value(mrb, value);
}

static mrb_value
mrb_sound_set_pan(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  SoundContainer *sound = get_sound(mrb, self);
  carbuncle_soloud.setPan(sound->handle, value);
  return mrb_float_value(mrb, value);
}

/**
 * Checks if the sound is dosposed
 * @return [Boolean]
 */
static mrb_value
mrb_sound_disposedQ(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(!DATA_PTR(self));
}

/**
 * Frees the memory associated with this resource.
 * Be aware, this object can't be used anymore or an error will be thrown.
 * @return [nil]
 */
static mrb_value
mrb_sound_dispose(mrb_state *mrb, mrb_value self)
{
  SoundContainer *sound = get_sound(mrb, self);
  mrb_sound_free(mrb, sound);
  DATA_PTR(self) = NULL;
  return self;
}

/**
 * Plays the current sound.
 * @return [nil]
 */
extern "C" mrb_value
mrb_sound_play(mrb_state *mrb, mrb_value self)
{
  SoundContainer *sound = get_sound(mrb, self);
  float volume = carbuncle_soloud.getVolume(sound->handle);
  float pan = carbuncle_soloud.getPan(sound->handle);
  SoLoud::handle h = carbuncle_soloud.play(sound->wav, volume, pan, false);
  carbuncle_soloud.setLooping(h, false);
  carbuncle_soloud.scheduleStop(h, sound->wav.getLength());
  return self;
}

/**
 * Pauses the sound, allowing to be resumed later.
 * @return [nil]
 */
extern "C" mrb_value
mrb_sound_pause(mrb_state *mrb, mrb_value self)
{
  SoundContainer *sound = get_sound(mrb, self);
  if (sound->playing)
  {
    sound->playing = FALSE;
    carbuncle_soloud.setPause(sound->handle, true);
  }
  return self;
}

/**
 * Stops the sound, setting it's position to the beggining.
 * @return [nil]
 */
extern "C" mrb_value
mrb_sound_stop(mrb_state *mrb, mrb_value self)
{
  SoundContainer *sound = get_sound(mrb, self);
  if (sound->playing)
  {
    sound->playing = FALSE;
    carbuncle_soloud.setPause(sound->handle, true);
  }
  carbuncle_soloud.seek(sound->handle, 0);
  return self;
}

extern "C" void
mrb_init_carbuncle_sound(mrb_state *mrb)
{
  struct RClass *carbuncle = mrb_carbuncle_get(mrb);
  /**
   * A sound is an audio file, which is streaming from the disk
   * instead of being loaded entirely into memory.
   * @!attribute [r] position
   *   The sound's position, in seconds.
   * @!attribute [r] size
   *   The sound's duration, in seconds.
   * @!attribute [r] length
   *   The sound's duration, in seconds.
   * @!attribute [r] count
   *   The sound's duration, in seconds.
   * @!attribute [rw] volume
   *   The sound's volume in the rangre from 0..1
   */
  struct RClass *sound = mrb_define_class_under(mrb, carbuncle, "Sound", mrb->object_class);
  MRB_SET_INSTANCE_TT(sound, MRB_TT_DATA);

  mrb_define_method(mrb, sound, "initialize", mrb_sound_initialize, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, sound, "pan", mrb_sound_get_pan, MRB_ARGS_NONE());
  mrb_define_method(mrb, sound, "size", mrb_sound_get_size, MRB_ARGS_NONE());
  mrb_define_method(mrb, sound, "length", mrb_sound_get_size, MRB_ARGS_NONE());
  mrb_define_method(mrb, sound, "count", mrb_sound_get_size, MRB_ARGS_NONE());
  mrb_define_method(mrb, sound, "volume", mrb_sound_get_volume, MRB_ARGS_NONE());

  mrb_define_method(mrb, sound, "volume=", mrb_sound_set_volume, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, sound, "pan=", mrb_sound_set_pan, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, sound, "disposed?", mrb_sound_disposedQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, sound, "dispose", mrb_sound_dispose, MRB_ARGS_NONE());

  mrb_define_method(mrb, sound, "play", mrb_sound_play, MRB_ARGS_NONE());
  mrb_define_method(mrb, sound, "pause", mrb_sound_pause, MRB_ARGS_NONE());
  mrb_define_method(mrb, sound, "stop", mrb_sound_stop, MRB_ARGS_NONE());
}
