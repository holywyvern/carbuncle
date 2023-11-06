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
        []
      end

      def zlib_library
        'libz.a'
      end

      def raylib_cmake_flags
        [
          '-DPLATFORM=Web',
          '-DSTATIC=ON',
          '-DBUILD_EXAMPLES=OFF',
          '-DBUILD_GAMES=OFF',
          '-DSUPPORT_FILEFORMAT_BMP=ON',
          '-DSUPPORT_FILEFORMAT_JPG=ON',
        ]
      end

      def soloud_cmake_flags
        emsdk = File.basename(`which emcc`)
        [
          '-DSOLOUD_BACKEND_SDL2=ON',
          "-DSDL2_INCLUDE_DIR=#{emsdk}/cache/include",
          "-DSDL2_LIBRARY=#{emsdk}/cache/ports-builds/libSDL2.a"
        ]
      end

      def libraries
        %w[raylib freetype physfs brotlienc brotlidec brotlicommon mbedtls mbedcrypto mbedx509 z xml2 tmx]
      end
  
      
      def toolchain_name
        'emcc'
      end

      def cmake_command
        'emcmake cmake'
      end

      def all_dependencies
        @all_dependencies ||= [
          raylib, zlib, freetype, brotli, libxml2, tmx, physfs, ssl, # ws
        ]
      end
    end
  end
end