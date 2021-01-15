module Carbuncle
  module Dependencies
    class PhysicsFS < Carbuncle::Dependency
      def url
        'https://github.com/Didstopia/physfs/archive/release-3.0.2.zip'
      end

      def subdir
        'physfs-release-3.0.2'
      end

      def lib_dir
        build_dir
      end

      def include_paths
        [File.join(base_dir, 'src')]
      end

      def name
        'PhysFS'
      end
    end
  end
end
