module Carbuncle
  module Dependencies
    class LibWebsockets < Carbuncle::Dependency
      def name
        'WS'
      end

      def url
        'https://github.com/warmcat/libwebsockets/archive/v3.1-stable.zip'
      end

      def subdir
        'libwebsockets-3.1-stable'
      end

      def lib_dir
        @lib_dir ||= File.join(build_dir, 'lib')
      end

      def include_paths
        [File.join(build_dir, 'include'), File.join(base_dir, 'include')]
      end
    end
  end
end
