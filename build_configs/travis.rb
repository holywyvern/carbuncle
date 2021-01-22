# frozen_string_literal: true

require_relative 'utils/gems'

MRuby::Build.new do |conf|
  conf.toolchain :gcc

  add_core_gems(conf)
  add_external_gems(conf)
  add_carbuncle_gems(conf)

  if ENV['DEBUG'] == 'true'
    conf.enable_test
    conf.enable_debug
    conf.cc.defines = %w[MRB_ENABLE_DEBUG_HOOK]
    conf.gem core: 'mruby-bin-debugger'
  end
end
