module Carbuncle
  class Dependency
    attr_reader :config

    def initialize(config)
      @config = config
    end

    def env
      @env ||= @config.env
    end

    def build
      return already_built if library_exists?

      download unless zip_exists?
      unzip
      build_dependency
    end

    def already_built
      puts "Library '#{name}' was already built."
    end

    def download
      puts("Downloading #{name}...")
      FileUtils.mkdir_p(vendor_dir)
      `wget -O #{zip_file} #{url}`
    end

    def unzip
      Zip::File.open(zip_file) do |zip|
        zip.each do |f|
          fpath = File.join(vendor_dir, f.name)
          FileUtils.mkdir_p(File.dirname(fpath))
          zip.extract(f, fpath) unless File.exist?(fpath)
        end
      end
    end

    def build_dependency
      puts "Building #{name}..."
      FileUtils.mkdir_p(build_dir)
      Dir.chdir(build_dir) do
        success = system("cmake #{cmake_flags.join(' ')} #{cmake_dir}") && system('cmake --build .')
        raise "Failed to build #{name}" unless success

        puts "#{name} was built!"
      end
    end

    def zip_file
      @zip_file ||= File.join(vendor_dir, "#{name}.zip")
    end

    def vendor_dir
      @vendor_dir ||= File.join(env.build.build_dir, 'vendor')
    end

    def cmake_flags
      config.general_cmake_flags + config.public_send(:"#{name.downcase}_cmake_flags")
    end

    def build_dir
      @build_dir ||= File.join(base_dir, 'build')
    end

    def base_dir
      @base_dir ||= File.join(vendor_dir, subdir)
    end

    def library_exists?
      File.exist?(library_file)
    end

    def library_file
      File.join(lib_dir, library_name)
    end

    def zip_exists?
      File.exist?(zip_file)
    end

    def library_name
      @library_name ||= config.public_send(:"#{name.downcase}_library")
    end

    def cmake_dir
      '..'
    end
  end
end
