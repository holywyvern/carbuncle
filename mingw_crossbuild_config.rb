# frozen_string_literal: true

require_relative 'build/gems'

MRuby::Build.new do |conf|
  conf.toolchain :gcc

  add_core_gems(conf)
end

MRuby::CrossBuild.new('mingw') do |conf|
  conf.toolchain :gcc

  conf.cc.command = 'x86_64-w64-mingw32-gcc'
  conf.cxx.command = 'x86_64-w64-mingw32-g++'
  conf.linker.command = 'x86_64-w64-mingw32-gcc'

  conf.linker.flags += %w[-static -static-libgcc]

  add_core_gems(conf)
  add_external_gems(conf)
  add_carbuncle_gems(conf)

  conf.cc.defines += %w[PCRE_STATIC]

  if ENV['DEBUG'] == 'true'
    conf.enable_test
    conf.enable_debug
    conf.cc.defines += %w[MRB_ENABLE_DEBUG_HOOK]
    conf.gem core: 'mruby-bin-debugger'
  end
end