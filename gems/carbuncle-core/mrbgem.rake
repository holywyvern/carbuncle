MRuby::Gem::Specification.new('carbuncle-core') do |spec|
  spec.license = 'MIT'
  spec.author  = 'Ramiro Rojo'
  spec.summary = 'Base carbuncle module'

  spec.add_dependency 'mruby-eval'
  spec.add_dependency 'carbuncle-support'
  spec.add_dependency 'carbuncle-dependencies'

  if spec.build.toolchains.include? 'gcc'
    spec.cc.flags << '-std=c11'
    spec.cxx.flags << '-std=c++11'
  end
end
