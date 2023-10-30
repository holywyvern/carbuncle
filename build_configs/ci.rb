# frozen_string_literal: true

require_relative 'utils/gems'

MRuby::Build.new do |conf|
  conf.toolchain :gcc

  setup_carbuncle(conf)

  conf.disable_presym
  conf.enable_test

  setup_carbuncle_debug(conf)
end
