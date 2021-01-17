module Carbuncle
  module Web
    class Emscripten < Carbuncle::Build
      def type
        'Web using Emscripten'
      end

      def toolchain_name
        'emcc'
      end

      def general_cmake_flags
        super +
          [
            "-DEMSCRIPTEN_ROOT_PATH=#{ENV['EMSCRIPTEN']}",
            '-G "Unix Makefiles"',
            '-DCMAKE_MAKE_PROGRAM=make'
          ]
      end

      def raylib_cmake_flags
        [
          '-DPLATFORM=Web',
          '-DSTATIC=ON',
          '-DBUILD_EXAMPLES=OFF',
          '-DBUILD_GAMES=OFF',
          '-DSUPPORT_FILEFORMAT_BMP=ON',
          '-DSUPPORT_FILEFORMAT_JPG=ON',
          '-DUSE_AUDIO=OFF' # We use SoLoud instead
        ]
      end

      def soloud_cmake_flags
        [
          '-DSOLOUD_BACKEND_SDL2=OFF',
          '-SOLOUD_BACKEND_OPENSLES=ON'
        ]
      end

      def toolchain_name
        'emcc'
      end
    end
  end
end