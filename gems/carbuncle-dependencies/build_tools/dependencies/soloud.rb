module Carbuncle
  module Dependencies
    class SoLoud < Carbuncle::Dependency
      def name
        'SoLoud'
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
