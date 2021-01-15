MRuby::Gem::Specification.new('carbuncle-graphics') do |spec|
  spec.license = 'MIT'
  spec.author  = 'Ramiro Rojo'
  spec.summary = 'Graphics module'

  spec.add_dependency 'carbuncle-core'
  spec.add_dependency 'carbuncle-math'

  if spec.build.toolchains.include? 'gcc'
    spec.cc.flags << '-std=c11'
    spec.cxx.flags << '-std=c++11'
  end
end
