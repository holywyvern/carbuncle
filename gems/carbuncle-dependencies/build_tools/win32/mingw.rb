module Carbuncle
  module Win32
    class MinGW < Carbuncle::Build
      def type
        'Windows using MinGW'
      end

      def general_cmake_flags
        return ['-G', '"MinGW Makefiles"'] if /mingw/i.match?(RUBY_PLATFORM)

        super
      end

      def libraries
        %w[
          raylib freetype physfs tmx soloud
          mbedtls mbedcrypto mbedx509
          xml2 zlibstatic kernel32 user32 shell32 winmm gdi32 opengl32 ws2_32
        ]
      end

      def toolchain_name
        'mingw'
      end

      def soloud_cmake_flags
        [
          '-DSOLOUD_BACKEND_SDL2=OFF',
          '-DSOLOUD_BACKEND_WINMM=ON'
        ]
      end
    end
  end
end
