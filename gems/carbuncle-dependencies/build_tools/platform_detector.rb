module Carbuncle
  class PlatformDetector
    attr_reader :env

    def initialize(env)
      @env = env
    end

    def detect
      return detect_by_cross_build if build.is_a?(MRuby::CrossBuild)

      detect_by_system
    end

    private

    def detect_by_cross_build
      return windows_build if /mingw/ =~ env.cc.command
      return emcc_build if /emcc/ =~ env.cc.command

      raise 'No cross build available detected for Carbuncle.'
    end

    def detect_by_system
      return osx_build if /darwin/ =~ RUBY_PLATFORM
      return windows_build if /mswin|mingw|win32/ =~ RUBY_PLATFORM

      linux_build
    end

    def osx_build
      return Carbuncle::OSX::CLang.new(env) if toolchains.include? 'clang'

      raise 'Carbuncle allows only CLang builds on MacOS for now.'
    end

    def windows_build
      return Carbuncle::Win32::MinGW.new(env) if toolchains.include? 'gcc'

      raise 'Carbuncle allows only MinGW builds for Windows for now.'
    end

    def linux_build
      return Carbuncle::Linux::GCC.new(env) if toolchains.include? 'gcc'

      raise 'Carbuncle only allows GCC Builds on Linux for now.'
    end

    def emcc_build
      Carbuncle::Web::Emscripten.new(env)
    end

    def build
      env.build
    end

    def build_dir
      env.build_dir
    end

    def toolchains
      @toolchains ||= build.toolchains
    end
  end
end
