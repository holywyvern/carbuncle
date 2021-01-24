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
          '-DSOLOUD_BACKEND_SDL2=ON',
          "-DSDL2_INCLUDE_DIR=#{ENV['EMSCRIPTEN']}/cache/include",
          "-DSDL2_LIBRARY=#{ENV['EMSCRIPTEN']}/cache/ports-builds/libSDL2.a"
        ]
      end

      def libraries
        %w[raylib freetype physfs soloud mbedtls mbedcrypto mbedx509 z xml2 tmx]
      end
  
      
      def toolchain_name
        'emcc'
      end

      def all_dependencies
        @all_dependencies ||= [
          raylib, zlib, freetype, libxml2, tmx, physfs, soloud, ssl, # ws
        ]
      end
    end
  end
end