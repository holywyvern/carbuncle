#include "carbuncle/core.h"
#include "carbuncle/music.h"

#include <mruby/class.h>
#include <mruby/data.h>

#include "soloud.h"
#include "soloud_wavstream.h"
#include "physfs_file.hpp"

extern SoLoud::Soloud carbuncle_soloud;

namespace
{
  struct MusicContainer
  {
    SoLoud::WavStream wav;
    PhysFsFile file;
    mrb_bool playing;
    SoLoud::handle handle;
  };
}

static void
mrb_music_free(mrb_state *mrb, void *p)
{
  if (p)
  {
    MusicContainer *music = reinterpret_cast<MusicContainer *>(p);
    carbuncle_soloud.stop(music->handle);
    delete music;
  }
}

static const struct mrb_data_type music_data_type = {
  "Carbuncle::Music", mrb_music_free
};

static MusicContainer *
get_music(mrb_state *mrb, mrb_value self)
{
  return DATA_GET_DISPOSABLE_PTR(mrb, self, &music_data_type, MusicContainer);
}

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
  MusicContainer *container = new MusicContainer();
  DATA_PTR(self) = container;
  DATA_TYPE(self) = &music_data_type;
  container->file.init(mrb, name);
  container->playing = FALSE;
  if (container->wav.loadFile(&container->file))
  {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Couldn't load music stream.");
  }
  container->handle = carbuncle_soloud.play(container->wav, -1, 0, true);
  carbuncle_soloud.setLooping(container->handle, true);
  return self;
}

/**
 * Checks if the sound is playing.
 * @return [Boolean]
 */
static mrb_value
mrb_music_playingQ(mrb_state *mrb, mrb_value self)
{
  MusicContainer *music = get_music(mrb, self);
  return mrb_bool_value(music->playing);
}

/**
 * @return [Float]
 */
static mrb_value
mrb_music_get_position(mrb_state *mrb, mrb_value self)
{
  MusicContainer *music = get_music(mrb, self);
  return mrb_float_value(mrb, carbuncle_soloud.getStreamTime(music->handle));
}

/**
 * @return [Float]
 */
static mrb_value
mrb_music_get_size(mrb_state *mrb, mrb_value self)
{
  MusicContainer *music = get_music(mrb, self);
  return mrb_float_value(mrb, music->wav.getLength());
}

/**
 * The music's volume in the rangre from 0..1
 * @return [Float]
 */
static mrb_value
mrb_music_get_volume(mrb_state *mrb, mrb_value self)
{
  MusicContainer *music = get_music(mrb, self);
  return mrb_float_value(mrb, carbuncle_soloud.getVolume(music->handle));
}

static mrb_value
mrb_music_get_pan(mrb_state *mrb, mrb_value self)
{
  MusicContainer *music = get_music(mrb, self);
  return mrb_float_value(mrb, carbuncle_soloud.getPan(music->handle));
}

static mrb_value
mrb_music_set_position(mrb_state *mrb, mrb_value self)
{
  MusicContainer *music = get_music(mrb, self);
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  carbuncle_soloud.seek(music->handle, value);
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
  MusicContainer *music = get_music(mrb, self);
  carbuncle_soloud.setVolume(music->handle, value);
  return mrb_float_value(mrb, value);
}

static mrb_value
mrb_music_set_pan(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  MusicContainer *music = get_music(mrb, self);
  carbuncle_soloud.setPan(music->handle, value);
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
  MusicContainer *music = get_music(mrb, self);
  mrb_music_free(mrb, music);
  DATA_PTR(self) = NULL;
  return self;
}

/**
 * Plays the current music.
 * @return [nil]
 */
extern "C" mrb_value
mrb_music_play(mrb_state *mrb, mrb_value self)
{
  MusicContainer *music = get_music(mrb, self);
  if (!music->playing)
  {
    music->playing = TRUE;
    carbuncle_soloud.setPause(music->handle, false);
  }
  return self;
}

/**
 * Pauses the music, allowing to be resumed later.
 * @return [nil]
 */
extern "C" mrb_value
mrb_music_pause(mrb_state *mrb, mrb_value self)
{
  MusicContainer *music = get_music(mrb, self);
  if (music->playing)
  {
    music->playing = FALSE;
    carbuncle_soloud.setPause(music->handle, true);
  }
  return self;
}

/**
 * Stops the music, setting it's position to the beggining.
 * @return [nil]
 */
extern "C" mrb_value
mrb_music_stop(mrb_state *mrb, mrb_value self)
{
  MusicContainer *music = get_music(mrb, self);
  if (music->playing)
  {
    music->playing = FALSE;
    carbuncle_soloud.setPause(music->handle, true);
  }
  carbuncle_soloud.seek(music->handle, 0);
  return self;
}

extern "C" void
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
  mrb_define_method(mrb, music, "size", mrb_music_get_size, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "length", mrb_music_get_size, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "count", mrb_music_get_size, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "volume", mrb_music_get_volume, MRB_ARGS_NONE());

  mrb_define_method(mrb, music, "volume=", mrb_music_set_volume, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, music, "position=", mrb_music_set_position, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, music, "pan=", mrb_music_set_pan, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, music, "disposed?", mrb_music_disposedQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "dispose", mrb_music_dispose, MRB_ARGS_NONE());

  mrb_define_method(mrb, music, "play", mrb_music_play, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "pause", mrb_music_pause, MRB_ARGS_NONE());
  mrb_define_method(mrb, music, "stop", mrb_music_stop, MRB_ARGS_NONE());
}
