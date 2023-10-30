# frozen_string_literal: true

require_relative 'utils/gems'
require_relative 'utils/empty_build'

MRuby::CrossBuild.new('mingw') do |conf|
  conf.toolchain :gcc

  conf.cc.command = 'x86_64-w64-mingw32-gcc'
  conf.cxx.command = 'x86_64-w64-mingw32-g++'
  conf.linker.command = 'x86_64-w64-mingw32-gcc'

  conf.linker.flags += %w[-static -static-libgcc]

  setup_carbuncle(conf)

  conf.cc.defines += %w[PCRE_STATIC]

  setup_carbuncle_debug(conf)
end