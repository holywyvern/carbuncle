#include "carbuncle/core.h"
#include "carbuncle/sound.h"
#include "carbuncle/filesystem.h"

#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/variable.h>

static void
mrb_sound_free(mrb_state *mrb, void *p)
{
  if (!p) return;
  Sound *sound = (Sound *)p;
  UnloadSound(*sound);
  mrb_free(mrb, p);
}

static const struct mrb_data_type sound_data_type = {
  "Carbuncle::Sound", mrb_sound_free
};

static Sound *
get_sound(mrb_state *mrb, mrb_value self)
{
  return DATA_GET_DISPOSABLE_PTR(mrb, self, &sound_data_type, Sound);
}

#define VOLUME mrb_intern_lit(mrb, "@volume")
#define PAN mrb_intern_lit(mrb, "@pan")
#define PITCH mrb_intern_lit(mrb, "@pitch")

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
  Sound *sound = mrb_malloc(mrb, sizeof *sound);
  *sound = LoadCarbuncleSound(mrb, name); 
  DATA_PTR(self) = sound;
  DATA_TYPE(self) = &sound_data_type;
  mrb_iv_set(mrb, self, VOLUME, mrb_float_value(mrb, 1));
  mrb_iv_set(mrb, self, PAN, mrb_float_value(mrb, 0));
  mrb_iv_set(mrb, self, PITCH, mrb_float_value(mrb, 1));  
  return self;
}

/**
 * Checks if the sound is playing.
 * @return [Boolean]
 */
static mrb_value
mrb_sound_playingQ(mrb_state *mrb, mrb_value self)
{
  Sound *sound = get_sound(mrb, self);
  return mrb_bool_value(IsSoundPlaying(*sound));
}

/**
 * The sound's volume in the rangre from 0..1
 * @return [Float]
 */
static mrb_value
mrb_sound_get_volume(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, VOLUME);
}

static mrb_value
mrb_sound_get_pan(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, PAN);
}

static mrb_value
mrb_sound_get_pitch(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, PITCH);
}

/**
 * @return [Float]
 */
static mrb_value
mrb_sound_set_volume(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  Sound *sound = get_sound(mrb, self);
  value = value < 0 ? 0 : (value > 1 ? 1 : value);
  SetSoundVolume(*sound, value);
  mrb_iv_set(mrb, self, VOLUME, mrb_float_value(mrb, value));
  return mrb_float_value(mrb, value);
}

static mrb_value
mrb_sound_set_pan(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  Sound *sound = get_sound(mrb, self);
  value = value < -1 ? -1 : (value > 1 ? 1 : value);
  SetSoundPan(*sound, value);
  mrb_iv_set(mrb, self, PAN, mrb_float_value(mrb, value));
  return mrb_float_value(mrb, value);
}

static mrb_value
mrb_sound_set_pitch(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  Sound *sound = get_sound(mrb, self);
  value = value < 0.5 ? 0.5 : (value > 2 ? 2 : value);
  SetSoundPitch(*sound, value);
  mrb_iv_set(mrb, self, PITCH, mrb_float_value(mrb, value));
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
  Sound *sound = get_sound(mrb, self);
  mrb_sound_free(mrb, sound);
  DATA_PTR(self) = NULL;
  return self;
}

/**
 * Plays the current sound.
 * @return [nil]
 */
mrb_value
mrb_sound_play(mrb_state *mrb, mrb_value self)
{
  Sound *sound = get_sound(mrb, self);
  if (!IsSoundPlaying(*sound))
  {
    PlaySound(*sound);
  }
  return self;
}

/**
 * Plays the current sound. From its paused position
 * @return [nil]
 */
mrb_value
mrb_sound_resume(mrb_state *mrb, mrb_value self)
{
  Sound *sound = get_sound(mrb, self);
  if (!IsSoundPlaying(*sound))
  {
    ResumeSound(*sound);
  }
  return self;
}

/**
 * Pauses the sound, allowing to be resumed later.
 * @return [nil]
 */
mrb_value
mrb_sound_pause(mrb_state *mrb, mrb_value self)
{
  Sound *sound = get_sound(mrb, self);
  if (IsSoundPlaying(*sound))
  {
    PauseSound(*sound);
  }
  return self;
}

/**
 * Stops the sound, setting it's position to the beggining.
 * @return [nil]
 */
mrb_value
mrb_sound_stop(mrb_state *mrb, mrb_value self)
{
  Sound *sound = get_sound(mrb, self);
  if (IsSoundPlaying(*sound))
  {
    StopSound(*sound);
  }
  return self;
}

void
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
  mrb_define_method(mrb, sound, "volume", mrb_sound_get_volume, MRB_ARGS_NONE());
  mrb_define_method(mrb, sound, "pitch", mrb_sound_get_pitch, MRB_ARGS_NONE());

  mrb_define_method(mrb, sound, "volume=", mrb_sound_set_volume, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, sound, "pan=", mrb_sound_set_pan, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, sound, "pitch=", mrb_sound_set_pitch, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, sound, "disposed?", mrb_sound_disposedQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, sound, "dispose", mrb_sound_dispose, MRB_ARGS_NONE());

  mrb_define_method(mrb, sound, "play", mrb_sound_play, MRB_ARGS_NONE());
  mrb_define_method(mrb, sound, "resume", mrb_sound_resume, MRB_ARGS_NONE());
  mrb_define_method(mrb, sound, "pause", mrb_sound_pause, MRB_ARGS_NONE());
  mrb_define_method(mrb, sound, "stop", mrb_sound_stop, MRB_ARGS_NONE());
}
