module Carbuncle
  module Dependencies
    class Freetype < Carbuncle::Dependency
      def name
        'Freetype'
      end

      def url
        'https://download.savannah.gnu.org/releases/freetype/ft2132.zip'
      end

      def subdir
        'freetype-2.13.2'
      end

      def lib_dir
        build_dir
      end

      def include_paths
        [File.join(build_dir, 'include'), File.join(base_dir, 'include')]
      end
    end
  end
end
