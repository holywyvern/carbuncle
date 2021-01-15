module Carbuncle
  module Linux
    class GCC < Carbuncle::Build
      def type
        'Linux using GCC'
      end

      def libraries
        %w[
          raylib freetype physfs tmx xml2 soloud
          mbedtls mbedcrypto mbedx509
          z m dl pthread X11 xcb GL Xext Xau Xdmcp
        ]
      end

      def toolchain_name
        'gcc'
      end

      def linker_flags
        [
          '-no-pie'
        ]
      end

      def zlib_library
        'libz.a'
      end

      def soloud_cmake_flags
        [
          "-DCMAKE_TOOLCHAIN_FILE=#{toolchain}",
          '-DSOLOUD_BACKEND_SDL2=OFF',
          '-DSOLOUD_BACKEND_ALSA=ON'
        ]
      end
    end
  end
end
