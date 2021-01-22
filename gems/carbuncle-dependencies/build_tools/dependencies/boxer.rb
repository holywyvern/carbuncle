module Carbuncle
    module Dependencies
      class Boxer < Carbuncle::Dependency
        def name
          'Boxer'
        end
  
        def url
          'https://github.com/aaronmjacobs/Boxer/archive/C.zip'
        end
  
        def subdir
          'Boxer-C'
        end
  
        def lib_dir
          build_dir
        end
  
        def include_paths
          [File.join(build_dir, 'include'), File.join(base_dir, 'include')]
        end
      end
    end
  end
  