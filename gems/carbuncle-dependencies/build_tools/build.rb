module Carbuncle
  class Build
    attr_reader :env

    def initialize(env)
      @env = env
      Carbuncle::Console.header("Detecting Carbuncle build for #{type}.")
    end

    def cmake_command
      'cmake'
    end

    def build_dependencies
      all_dependencies.each(&:build)
    end

    def zlib
      @zlib ||= Carbuncle::Dependencies::Zlib.new(self)
    end

    def freetype
      @freetype ||= Carbuncle::Dependencies::Freetype.new(self)
    end

    def raylib
      @raylib ||= Carbuncle::Dependencies::Raylib.new(self)
    end

    def libxml2
      @libxml2 ||= Carbuncle::Dependencies::LibXML2.new(self)
    end

    def tmx
      @tmx ||= Carbuncle::Dependencies::TMX.new(self)
    end

    def physfs
      @physfs ||= Carbuncle::Dependencies::PhysicsFS.new(self)
    end

    def soloud
      @soloud ||= Carbuncle::Dependencies::SoLoud.new(self)
    end

    def ssl
      @ssl ||= Carbuncle::Dependencies::MbedTLS.new(self)
    end

    def ws
      @ws ||= Carbuncle::Dependencies::LibWebsockets.new(self)
    end

    def boxer
      @boxer ||= Carbuncle::Dependencies::Boxer.new(self)
    end

    def library_paths
      all_dependencies.map(&:lib_dir)
    end

    def libraries
      %w[raylib freetype physfs soloud mbedtls mbedcrypto mbedx509 z xml2 tmx Boxer]
    end

    def general_cmake_flags
      [
        "-DCMAKE_TOOLCHAIN_FILE=#{toolchain}"
      ]
    end

    def tmx_cmake_flags
      [
        "-DZLIB_ROOT=#{zlib.base_dir}",
        "-DZLIB_LIBRARY=#{File.join(zlib.build_dir, zlib_library)}",
        "-DZLIB_INCLUDE_DIR=#{zlib.base_dir}",
        "-DLIBXML2_LIBRARIES=#{File.join(libxml2.build_dir, libxml2_library)}",
        "-DLIBXML2_LIBRARY=#{File.join(libxml2.build_dir, libxml2_library)}",
        "-DLIBXML2_INCLUDE_DIR=#{libxml2.include_paths.join(',')}",
        "-DLIBXML2_INCLUDE_DIRS=#{libxml2.include_paths.join(',')}"
      ]
    end

    def zlib_cmake_flags
      []
    end

    def libxml2_cmake_flags
      [
        '-DBUILD_SHARED_LIBS=OFF',
        "-DZLIB_ROOT=#{zlib.base_dir}",
        "-DZLIB_LIBRARY=#{File.join(zlib.build_dir, zlib_library)}",
        "-DZLIB_INCLUDE_DIR=#{zlib.base_dir}",
        '-DLIBXML2_WITH_ICONV=OFF',
        '-DLIBXML2_WITH_HTML=OFF',
        '-DLIBXML2_WITH_LZMA=OFF'
      ]
    end

    def raylib_cmake_flags
      [ 
        '-DCUSTOMIZE_BUILD=ON',
        '-DPLATFORM=Desktop',
        '-DBUILD_EXAMPLES=OFF',
        '-DSUPPORT_FILEFORMAT_BMP=ON',
        '-DSUPPORT_FILEFORMAT_JPG=ON',
        '-DUSE_AUDIO=OFF' # We use SoLoud instead
      ]
    end

    def freetype_cmake_flags
      [
        '-DCMAKE_DISABLE_FIND_PACKAGE_PNG=ON',
        '-DCMAKE_DISABLE_FIND_PACKAGE_HarfBuzz=ON',
        '-DCMAKE_DISABLE_FIND_PACKAGE_BZip2=ON',
        "-DZLIB_ROOT=#{zlib.base_dir}",
        "-DZLIB_LIBRARY=#{File.join(zlib.build_dir, zlib_library)}",
        "-DZLIB_INCLUDE_DIR=#{zlib.base_dir}"
      ]
    end

    def physfs_cmake_flags
      [
        '-DPHYSFS_BUILD_SHARED=FALSE'
      ]
    end

    def soloud_cmake_flags
      []
    end

    def ssl_cmake_flags
      [
        "-DZLIB_ROOT=#{zlib.base_dir}",
        "-DZLIB_LIBRARY=#{File.join(zlib.build_dir, zlib_library)}",
        "-DZLIB_INCLUDE_DIR=#{zlib.base_dir}",
        '-DENABLE_ZLIB_SUPPORT=ON',
        '-DENABLE_TESTING=OFF',
        '-DENABLE_PROGRAMS=OFF'
      ]
    end

    def ws_cmake_flags
      [
        "-DZLIB_ROOT=#{zlib.base_dir}",
        "-DZLIB_LIBRARY=#{File.join(zlib.build_dir, zlib_library)}",
        "-DZLIB_INCLUDE_DIR=#{zlib.base_dir}",
        '-DLWS_WITH_STATIC=ON',
        '-DLWS_WITH_SHARED=OFF',
        '-DLWS_WITHOUT_TESTAPPS=ON',
        '-DLWS_WITHOUT_TEST_SERVER=ON',
        '-DLWS_WITHOUT_TEST_SERVER_EXTPOLL=ON',
        '-DLWS_WITHOUT_TEST_PING=ON',
        '-DLWS_WITHOUT_TEST_CLIENT=ON',
        '-DLWS_WITH_MBEDTLS=ON',
        "-DLWS_MBEDTLS_INCLUDE_DIRS=#{ssl.include_paths.first}",
        "-DMBEDTLS_LIBRARY=#{File.join(ssl.lib_dir, ssl_library)}",
        "-DMBEDX509_LIBRARY=#{File.join(ssl.lib_dir, x509_library)}",
        "-DMBEDCRYPTO_LIBRARY=#{File.join(ssl.lib_dir, crypto_library)}"
      ]
    end

    def boxer_cmake_flags
      []
    end

    def include_paths
      all_dependencies.map(&:include_paths).flatten
    end

    def toolchain
      @toolchain ||= File.join(env.dir, 'build_tools', 'toolchains', "#{toolchain_name}.cmake")
    end

    def flags
      ['-DZLIB_WINAPI']
    end

    def linker_flags
      []
    end

    def library_extension
      'a'
    end

    def freetype_library
      'libfreetype.a'
    end

    def raylib_library
      'libraylib.a'
    end

    def zlib_library
      'libzlibstatic.a'
    end

    def libxml2_library
      'libxml2.a'
    end

    def tmx_library
      'libtmx.a'
    end

    def physfs_library
      'libphysfs.a'
    end

    def soloud_library
      'libsoloud.a'
    end

    def ws_library
      'libwebsockets.a'
    end

    def ssl_library
      'libmbedtls.a'
    end

    def crypto_library
      'libmbedcrypto.a'
    end

    def x509_library
      'libmbedx509.a'
    end

    def boxer_library
      'libBoxer.a'
    end

    def all_dependencies
      @all_dependencies ||= [
        raylib, zlib, freetype, libxml2, tmx, physfs, soloud, ssl, boxer, # ws
      ]
    end
  end
end
