# frozen_string_literal: true

MRUBY_CONFIG = File.expand_path(ENV['MRUBY_CONFIG'] || '.travis_build_config.rb')
MRUBY_VERSION = ENV['MRUBY_VERSION'] || '2.1.2'

file :mruby do
  sh 'git clone --depth=1 git://github.com/mruby/mruby.git'
  if MRUBY_VERSION != 'master'
    Dir.chdir 'mruby' do
      sh 'git fetch --tags'
      rev = `git rev-parse #{MRUBY_VERSION}`
      sh "git checkout #{rev}"
    end
  end
end

desc 'compile binary'
task compile: :mruby do
  sh "cd mruby && rake all MRUBY_CONFIG=#{MRUBY_CONFIG}"
end

desc 'test'
task test: :mruby do
  sh "cd mruby && rake all test MRUBY_CONFIG=#{MRUBY_CONFIG}"
end

desc 'cleanup'
task :clean do
  exit 0 unless File.directory?('mruby')
  sh 'cd mruby && rake deep_clean'
end

desc 'move bin folder'
task bins: :compile do
  dst_bin = File.join(__dir__, 'mruby', 'bin')
  FileUtils.mv(dst_bin, __dir__, verbose: true, force: true)
end

task default: :bins
