module Carbuncle
  module Dependencies
    class Raylib < Carbuncle::Dependency
      def name
        'Raylib'
      end

      def url
        'https://github.com/raysan5/raylib/archive/3.5.0.zip'
      end

      def subdir
        'raylib-3.5.0'
      end

      def lib_dir
        @lib_dir ||= File.join(build_dir, 'src')
      end

      def include_paths
        [base_dir, File.join(base_dir, 'src'),
         build_dir, File.join(build_dir, 'src')]
      end
    end
  end
end
