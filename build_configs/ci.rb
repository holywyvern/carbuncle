# frozen_string_literal: true

require_relative 'utils/gems'

MRuby::Build.new do |conf|
  conf.toolchain :gcc

  add_core_gems(conf)
  add_external_gems(conf)
  add_carbuncle_gems(conf)

  conf.disable_presym
  conf.enable_test

  if ENV['DEBUG'] == 'true'
    conf.enable_debug
    conf.cc.defines = %w[MRB_ENABLE_DEBUG_HOOK]
    conf.gem core: 'mruby-bin-debugger'
  end
end
