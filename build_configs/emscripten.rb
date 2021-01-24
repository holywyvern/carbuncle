# frozen_string_literal: true

require_relative 'utils/gems'

# Build an empty ruby build on current target
MRuby::Build.new do |conf|
  conf.toolchain :gcc
end

MRuby::CrossBuild.new('web') do |conf|
  conf.toolchain :clang

  emscripten_flags = [
    '-s ASYNCIFY=1',
    '-s USE_GLFW=3',
    '-s USE_SDL=2',
    '-s USE_SDL_MIXER=2',
    '--use-preload-plugins',
    '-s FORCE_FILESYSTEM=1',
    '-s DISABLE_EXCEPTION_CATCHING=0',
    '-s ALLOW_MEMORY_GROWTH=1',
    '-O3',
    "--shell-file #{File.join(__dir__, 'utils', 'emscripten_shell.html')}"
  ]

  conf.gembox 'default'
  conf.cc.command = 'emcc'
  conf.cxx.command = 'em++'
  conf.archiver.command = 'emar'
  conf.cc.flags += emscripten_flags
  conf.cxx.flags += emscripten_flags
  conf.exts.executable = '.html'
  conf.linker.command = 'emcc'
  conf.linker.flags = emscripten_flags
  

  add_core_gems(conf)
  add_external_gems(conf)
  add_carbuncle_gems(conf)  
end
