#include "carbuncle/core.h"
#include "carbuncle/music.h"
#include "carbuncle/filesystem.h"

#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/variable.h>

static void
mrb_music_free(mrb_state *mrb, void *p)
{
  if (!p) return;

  Music *music = (Music *)p;
  UnloadMusicStream(*music);
  mrb_free(mrb, p);
}

static const struct mrb_data_type music_data_type = {
  "Carbuncle::Music", mrb_music_free
};

static Music *
get_music(mrb_state *mrb, mrb_value self)
{
  return DATA_GET_DISPOSABLE_PTR(mrb, self, &music_data_type, Music);
}

#define VOLUME mrb_intern_lit(mrb, "@volume")
#define PAN mrb_intern_lit(mrb, "@pan")
#define PITCH mrb_intern_lit(mrb, "@pitch")

/**
 * @overload initialize(filename)
 *   Creates a new Music object from a filename
 *   @param [String] filename The name of the audio file.
 *   @raise [Carbuncle::FileNotFound] If the file does not exists.
 *   @return [self]
 */
static mrb_value
mrb_music_initialize(mrb_state *mrb, mrb_value self)
{
  const char *name;
  mrb_get_args(mrb, "z", &name);
  mrb_carbuncle_check_file(mrb, name);
  Music *music = mrb_malloc(mrb, sizeof *music);
  *music = LoadCarbuncleMusic(mrb, name); 
  DATA_PTR(self) = music;
  DATA_TYPE(self) = &music_data_type;
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
mrb_music_playingQ(mrb_state *mrb, mrb_value self)
{
  Music *music = get_music(mrb, self);
  return mrb_bool_value(IsMusicStreamPlaying(*music));
}

/**
 * @return [Float]
 */
static mrb_value
mrb_music_get_position(mrb_state *mrb, mrb_value self)
{
  Music *music = get_music(mrb, self);
  return mrb_float_value(mrb, GetMusicTimePlayed(*music));
}

/**
 * @return [Float]
 */
static mrb_value
mrb_music_get_size(mrb_state *mrb, mrb_value self)
{
  Music *music = get_music(mrb, self);
  return mrb_float_value(mrb, GetMusicTimeLength(*music));
}

/**
 * The music's volume in the rangre from 0..1
 * @return [Float]
 */
static mrb_value
mrb_music_get_volume(mrb_state *mrb, mrb_value self)
{
  get_music(mrb, self);
  return mrb_iv_get(mrb, self, VOLUME);
}

static mrb_value
mrb_music_get_pan(mrb_state *mrb, mrb_value self)
{
  get_music(mrb, self);
  return mrb_iv_get(mrb, self, PAN);
}

static mrb_value
mrb_music_get_pitch(mrb_state *mrb, mrb_value self)
{
  get_music(mrb, self);
  return mrb_iv_get(mrb, self, PITCH);
}


static mrb_value
mrb_music_set_position(mrb_state *mrb, mrb_value self)
{
  Music *music = get_music(mrb, self);
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  SeekMusicStream(*music, value);
  return mrb_float_value(mrb, value);
}

/**
 * @return [Float]
 */
static mrb_value
mrb_music_set_volume(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  Music *music = get_music(mrb, self);
  value = value < 0 ? 0 : (value > 1 ? 1 : value);
  SetMusicVolume(*music, value);
  mrb_iv_set(mrb, self, VOLUME, mrb_float_value(mrb, value));
  return mrb_float_value(mrb, value);
}

static mrb_value
mrb_music_set_pan(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  Music *music = get_music(mrb, self);
  value = value < -1 ? -1 : (value > 1 ? 1 : value);
  SetMusicPan(*music, value);
  mrb_iv_set(mrb, self, PAN, mrb_float_value(mrb, value));
  return mrb_float_value(mrb, value);
}

static mrb_value
mrb_music_set_pitch(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  Music *music = get_music(mrb, self);
  value = value < 0.5 ? 0.5 : (value > 2 ? 2 : value);
  SetMusicPitch(*music, value);
  mrb_iv_set(mrb, self, PITCH, mrb_float_value(mrb, value));
  return mrb_float_value(mrb, value);
}

/**
 * Checks if the sound is dosposed
 * @return [Boolean]
 */
static mrb_value
mrb_music_disposedQ(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(!DATA_PTR(self));
}

/**
 * Frees the memory associated with this resource.
 * Be aware, this object can't be used anymore or an error will be thrown.
 * @return [nil]
 */
static mrb_value
mrb_music_dispose(mrb_state *mrb, mrb_value self)
{
  Music *music = get_music(mrb, self);
  mrb_music_free(mrb, music);
  DATA_PTR(self) = NULL;
  return self;
}

/**
 * Plays the current music.
 * @return [nil]
 */
mrb_value
mrb_music_play(mrb_state *mrb, mrb_value self)
{
  Music *music = get_music(mrb, self);
  if (!IsMusicStreamPlaying(*music))
  {
    PlayMusicStream(*music);
  }
  return self;
}

/**
 * Plays the current music at the place it was paused.
 * @return [nil]
 */
mrb_value
mrb_music_resume(mrb_state *mrb, mrb_value self)
{
  Music *music = get_music(mrb, self);
  if (!IsMusicStreamPlaying(*music))
  {
    ResumeMusicStream(*music);
  }
  return self;
}

/**
 * Pauses the music, allowing to be resumed later.
 * @return [nil]
 */
mrb_value
mrb_music_pause(mrb_state *mrb, mrb_value self)
{
  Music *music = get_music(mrb, self);
  if (IsMusicStreamPlaying(*music))
  {
    PauseMusicStream(*music);
  }
  return self;
}

/**
 * Stops the music, setting it's position to the beggining.
 * @return [nil]
 */
mrb_value
mrb_music_stop(mrb_state *mrb, mrb_value self)
{
  Music *music = get_music(mrb, self);
  if (IsMusicStreamPlaying(*music))
  {
    StopMusicStream(*music);
  }
  return self;
}

static mrb_value
mrb_music_update(mrb_state *mrb, mrb_value self)
{
  Music *music = get_music(mrb, self);
  UpdateMusicStream(*music);
  return self;
}

void
mrb_init_carbuncle_music(mrb_state *mrb)
{
  struct RClass *carbuncle = mrb_carbuncle_get(mrb);
  /**
   * A Music is an audio file, which is streaming from the disk
   * instead of being loaded entirely into memory.
   * @!attribute [r] position
   *   The music's position, in seconds.
   * @!attribute [r] size
   *   The music's duration, in seconds.
   * @!attribute [r] length
   *   The music's duration, in seconds.
   * @!attribute [r] count
   *   The music's duration, in seconds.
   * @!attribute [rw] volume
   *   The music's volume in the rangre from 0..1
   */
  struct RClass *music = mrb_define_class_under(mrb, carbuncle, "Music", mrb->object_class);
  MRB_SET_INSTANCE_TT(music, MRB_TT_DATA);

  mrb_define_method(mrb, music, "initialize", mrb_music_initialize, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, music, "playing?", mrb_music_playingQ, MRB_ARGS_NONE());

  mrb_define_method(mrb, music, "position", mrb_music_get_position, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "pan", mrb_music_get_pan, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "pitch", mrb_music_get_pitch, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "size", mrb_music_get_size, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "length", mrb_music_get_size, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "count", mrb_music_get_size, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "volume", mrb_music_get_volume, MRB_ARGS_NONE());

  mrb_define_method(mrb, music, "volume=", mrb_music_set_volume, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, music, "position=", mrb_music_set_position, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, music, "pan=", mrb_music_set_pan, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, music, "pitch=", mrb_music_set_pitch, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, music, "disposed?", mrb_music_disposedQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "dispose", mrb_music_dispose, MRB_ARGS_NONE());

  mrb_define_method(mrb, music, "play", mrb_music_play, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "resume", mrb_music_resume, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "pause", mrb_music_pause, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "stop", mrb_music_stop, MRB_ARGS_NONE());

  mrb_define_method(mrb, music, "update", mrb_music_update, MRB_ARGS_NONE());
}
