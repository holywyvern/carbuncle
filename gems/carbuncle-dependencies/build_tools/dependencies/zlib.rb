module Carbuncle
  module Dependencies
    class Zlib < Carbuncle::Dependency
      def build
        super
        FileUtils.cp(File.join(build_dir, 'zconf.h'), File.join(base_dir, 'zconf.h'))
      end

      def name
        'Zlib'
      end

      def url
        'https://github.com/madler/zlib/releases/download/v1.3/zlib13.zip'
      end

      def subdir
        'zlib-1.3'
      end

      def lib_dir
        build_dir
      end

      def include_paths
        [base_dir]
      end

      def libraries
        config.zlib_libraries
      end
    end
  end
end
