CARBUNCLE_GEMS = %w[
  dependencies support core audio doc-ext graphics
  input math scene
].freeze
  
CORE_GEMS = %w[
  mruby-metaprog mruby-pack mruby-sprintf mruby-print mruby-math
  mruby-time mruby-struct mruby-compar-ext mruby-enum-ext
  mruby-string-ext mruby-numeric-ext mruby-array-ext mruby-hash-ext
  mruby-range-ext mruby-proc-ext mruby-symbol-ext mruby-random
  mruby-object-ext mruby-objectspace mruby-fiber mruby-enumerator
  mruby-enum-lazy mruby-toplevel-ext mruby-rational mruby-complex
  mruby-kernel-ext mruby-class-ext mruby-method mruby-eval
  mruby-compiler
].freeze
  
GITHUB_GEMS = %w[
  yui-knk/mruby-set iij/mruby-regexp-pcre mattn/mruby-json
].freeze
  
def add_core_gems(conf)
  CORE_GEMS.each do |gem|
    conf.gem core: gem
  end
end
  
def add_external_gems(conf)
  GITHUB_GEMS.each do |gem|
    conf.gem github: gem
  end
end
  
def add_carbuncle_gems(conf)
  root_dir = File.join(__dir__, '..', '..', 'gems')
  CARBUNCLE_GEMS.each do |name|
    conf.gem File.expand_path(File.join(root_dir, "carbuncle-#{name}"))
  end
  conf.gem File.expand_path(File.join(root_dir, 'mruby-bin-carbuncle'))
end
