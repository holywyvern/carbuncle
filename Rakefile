# frozen_string_literal: true

MRUBY_CONFIG = File.expand_path(ENV['MRUBY_CONFIG'] || File.join('build_configs', 'travis.rb'))
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
  dst_folder = File.join(__dir__, 'bin')
  FileUtils.mkdir_p(dst_folder)
  Dir[File.join(__dir__, 'mruby', 'bin', '*')].each do |src|
    name = File.basename(src)
    dst = File.join(dst_folder, name)
    FileUtils.rm_r(dst) if File.exist?(dst)
    FileUtils.cp_r(src, dst)
  end
end

task default: :bins
