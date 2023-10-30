# frozen_string_literal: true

# Build an empty ruby build on current target
MRuby::Build.new do |conf|
  conf.toolchain :gcc
end