module Carbuncle
  module Dependencies
    class Brotli < Carbuncle::Dependency
      def name
        'Brotli'
      end

      def url
        'https://github.com/google/brotli/archive/refs/tags/v1.1.0.zip'
      end

      def subdir
        'brotli-1.1.0'
      end

      def lib_dir
        build_dir
      end

      def include_paths
        [File.join(build_dir, 'include'), File.join(base_dir, 'c', 'include')]
      end
    end
  end
end
