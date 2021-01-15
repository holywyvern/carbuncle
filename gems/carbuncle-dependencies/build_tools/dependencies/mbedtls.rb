module Carbuncle
  module Dependencies
    class MbedTLS < Carbuncle::Dependency
      def name
        'SSL'
      end

      def url
        'https://github.com/ARMmbed/mbedtls/archive/mbedtls-2.16.3.zip'
      end

      def subdir
        'mbedtls-mbedtls-2.16.3'
      end

      def lib_dir
        @lib_dir ||= File.join(build_dir, 'library')
      end

      def include_paths
        [File.join(base_dir, 'include')]
      end
    end
  end
end
