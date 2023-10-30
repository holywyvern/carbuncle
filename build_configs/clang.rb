# frozen_string_literal: true

require_relative 'utils/gems'

MRuby::Build.new do |conf|
  conf.toolchain :clang

  setup_carbuncle(conf)

  conf.enable_test
  conf.disable_presym

  setup_carbuncle_debug(conf)
end
