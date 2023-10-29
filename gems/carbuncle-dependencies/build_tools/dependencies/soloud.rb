module Carbuncle
  module Dependencies
    class SoLoud < Carbuncle::Dependency
      def name
        'SoLoud'
      end

      def unzip
        super
        check = <<~CHECK
          if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
            add_link_options(
              -s ASYNCIFY=1
              -s USE_GLFW=3
              -s USE_SDL=2
              -s USE_SDL_MIXER=2
              --use-preload-plugins
              -s FORCE_FILESYSTEM=1
              -s DISABLE_EXCEPTION_CATCHING=0
              -s ALLOW_MEMORY_GROWTH=1
              -O3
            )
          endif()
        CHECK
        File.open(File.join(base_dir, 'contrib', 'CMakeLists.txt'), 'a') do |f|
          f << check
        end
      end
  
      def url
        'https://github.com/jarikomppa/soloud/archive/RELEASE_20200207.zip'
      end

      def subdir
        'soloud-RELEASE_20200207'
      end

      def lib_dir
        build_dir
      end

      def include_paths
        [File.join(base_dir, 'include')]
      end

      def libraries
        'soloud'
      end

      def cmake_dir
        File.join('..', 'contrib')
      end

      def build_dir
        @build_dir ||= File.join(base_dir, 'cmake_build')
      end
    end
  end
end
