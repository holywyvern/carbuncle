# frozen_string_literal: true

MRUBY_CONFIG = File.expand_path(ENV['MRUBY_CONFIG'] || File.join('build_configs', 'ci.rb'))
MRUBY_VERSION = ENV['MRUBY_VERSION'] || '2.1.2'

require_relative 'build_configs/utils/gems'

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

task :doc_files do
  GITHUB_GEMS.each do |gem|
    sh "git clone https://github.com/#{gem}.git .doc_tmp/#{gem}"
  end
end

desc 'generate documentation'
task docs: :doc_files do
  puts 'hello world'
  gem_list = CARBUNCLE_GEMS.map {|i| "carbuncle-#{i}" }.join(',')
  core_gems = CORE_GEMS.join(',')
  carbuncle_dirs = "gems/{#{gem_list}}/{src,mrblib}/{**}/*.{rb,c}"
  mruby_dirs = "mruby/{src,mrblib}/*.{c,rb} mruby/mrbgems/{#{core_gems}}/{src, mrblib}/*.{c,rb}"
  github_dirs = ".doc_tmp/**/{src,mrblib}/*.{c,rb}"
  gem_files = "#{carbuncle_dirs} #{mruby_dirs} #{github_dirs}"
  sh "mrbdoc #{gem_files} -o website/out/docs"
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
