module Carbuncle
  module Dependencies
    class LibXML2 < Carbuncle::Dependency
      def name
        'LibXML2'
      end

      def url
        'https://github.com/robotology-dependencies/libxml2-cmake-buildsystem/archive/master.zip'
      end

      def subdir
        'libxml2-cmake-buildsystem-master'
      end

      def lib_dir
        build_dir
      end

      def include_paths
        [deps_dir]
      end

      def build_dependency
        FileUtils.cp(
          File.join(__dir__, 'libxml2', 'GenerateConfigHeader.cmake'),
          File.join(base_dir, 'GenerateConfigHeader.cmake')
        )
        super
        FileUtils.cp(
          File.join(build_dir, 'libxml', 'xmlversion.h'),
          File.join(deps_dir, 'libxml', 'xmlversion.h')
        )
      end

      def deps_dir
        @deps_dir ||= File.join(build_dir, '_deps', 'libxml2upstream-src', 'include')
      end
    end
  end
end
