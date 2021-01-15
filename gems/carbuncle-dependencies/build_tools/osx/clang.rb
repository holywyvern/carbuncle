module Carbuncle
  module OSX
    class CLang < Carbuncle::Build
      def type
        'Mac OSX using CLang'
      end

      def flags
        []
      end

      def linker_flags
        [
          '-framework OpenGL',
          '-framework OpenAL',
          '-framework IOKit',
          '-framework CoreVideo',
          '-framework CoreAudio',
          '-framework AudioToolbox',
          '-framework Cocoa'
        ]
      end

      def toolchain_name
        'clang'
      end

      def soloud_cmake_flags
        [
          "-DCMAKE_TOOLCHAIN_FILE=#{toolchain}",
          '-DSOLOUD_BACKEND_SDL2=OFF',
          '-DSOLOUD_BACKEND_COREAUDIO=ON'
        ]
      end
    end
  end
end
