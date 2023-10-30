CARBUNCLE_GEMS = %w[
  dependencies support core audio doc-ext graphics
  input math scene tilemap
].freeze

STDLIB_GEMS = %w[
  mruby-compar-ext mruby-enum-ext mruby-string-ext mruby-numeric-ext
  mruby-array-ext mruby-hash-ext mruby-range-ext mruby-proc-ext
  mruby-symbol-ext mruby-object-ext mruby-objectspace mruby-set
  mruby-fiber mruby-enumerator mruby-enum-lazy mruby-toplevel-ext
  mruby-kernel-ext mruby-class-ext mruby-struct mruby-catch mruby-data
  mruby-enum-chain mruby-pack mruby-proc-binding
].freeze

MATH_GEMS = %w[
  mruby-math mruby-rational mruby-complex mruby-cmath mruby-bigint
  mruby-random
].freeze

METAPROG_GEMS = %w[
  mruby-metaprog mruby-method mruby-eval mruby-compiler
].freeze

MRUBY_BINARIES = %w[
  mruby-bin-mrbc mruby-bin-mirb
].freeze

CORE_GEMS = (STDLIB_GEMS + MATH_GEMS + METAPROG_GEMS + MRUBY_BINARIES).freeze
  
GITHUB_GEMS = %w[
  iij/mruby-regexp-pcre mattn/mruby-json
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
