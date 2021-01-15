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
        'http://zlib.net/zlib1211.zip'
      end

      def subdir
        'zlib-1.2.11'
      end

      def lib_dir
        build_dir
      end

      def include_paths
        [base_dir]
      end

      def libraries
        'zlib'
      end
    end
  end
end
