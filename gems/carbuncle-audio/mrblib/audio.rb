module Carbuncle
  module Audio
    class << self

      def _fades
        @_fades ||= {}
      end

      def _musics
        @_musics ||= {}
      end

      def _sounds
        @_sounds ||= []
        @_sounds.each do |sound|
          sound.dispose unless sound.playing?
        end
        @_sounds = @_sounds.filter { |i| !i.disposed? }
        @_sounds
      end

      def _pause_music
        return if @music_playing.present?

        @music_playing = {}
        _musics.each_key do |channel|
          music = _musics[channel]
          @music_playing[channel] = music.playing?
          music.pause if music.playing?
        end
      end

      def _resume_music
        _musics.each_key do |channel|
          if @music_playing[channel]
            music.resume
          end
        end
        @music_playing = nil
      end

      def music_play(name, volume = 1.0, pitch = 1.0, pos = 0, channel = 0)
        old_music = _musics[channel]
        if old_music.present?
          old_music.stop if old_music.playing?
          old_music.dispose
        end
        music = Carbuncle::Music.new(name)
        music.volume = volume
        music.pitch = pitch
        music.position = pos
        music.play
        _musics[channel] = music
        self
      end

      def music_stop(channel)
        old_music = _musics[channel]
        return self unless old_music.present?

        old_music.stop if old_music.playing?
        old_music.dispose
        _musics.delete(channel)
        self
      end

      def music_fade(channel, time)
        _fades[channel] = time
        self
      end

      def music_position(channel)
        return _musics[channel].position if _musics[channel].present?

        0
      end

      def bgm_play(name, volume = 1.0, pitch = 1.0, pos = 0)
        music_play(name, volume, pitch, pos, 0)
      end

      def bgm_stop
        music_stop(0)
      end

      def bgm_pos
        music_position(0)
      end

      def bgm_fade(time)
        music_fade(0, time)
      end

      def bgs_play(name, volume = 1.0, pitch = 1.0, pos = 0)
        music_play(name, volume, pitch, pos, 1)
      end

      def bgs_stop
        music_stop(1)
      end

      def bgs_pos
        music_position(1)
      end

      def bgs_fade(time)
        music_fade(0, time)
      end

      def me_play(name, volume = 1.0, pitch = 1.0)
        if @me.present?
          @me.stop
          @me.dispose
        end
        _pause_music
        @me = Sound.new(name)
        @me.volume = volume
        @me.pitch = pitch
        @me.play
        self
      end

      def sound_play(name, volume = 1.0, pitch = 1.0)
        sound = Carbuncle::Sound.new(name)
        sound.volume = volume
        sound.pitch = pitch
        sound.play
        _sounds << sound
        self
      end

      def se_play(name, volume = 1.0, pitch = 1.0)
        sound_play(name, volume, pitch)
      end

      def se_stop
        _sounds.each do |sound|
          sound.stop
          sound.dispose
        end
        _sounds.clear
        self
      end

      def update(dt)
        if @me.present? && !@me.playing?
          @me.dispose
          @me = nil
          _resume_music
        end
        _musics.each_value(&:update)
        self
      end
    end
  end
end
