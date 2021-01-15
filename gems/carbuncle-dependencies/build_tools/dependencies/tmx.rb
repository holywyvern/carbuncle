module Carbuncle
  module Dependencies
    class TMX < Carbuncle::Dependency
      def name
        'TMX'
      end

      def url
        'https://github.com/holywyvern/tmx/archive/feature-xml_reader_override.zip'
      end

      def subdir
        'tmx-feature-xml_reader_override'
      end

      def lib_dir
        build_dir
      end

      def include_paths
        [File.join(base_dir, 'src')]
      end

      def libraries
        'tmx'
      end
    end
  end
end
