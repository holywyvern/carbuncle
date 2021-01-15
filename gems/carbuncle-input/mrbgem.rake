MRuby::Gem::Specification.new('carbuncle-input') do |spec|
  spec.license = 'MIT'
  spec.author  = 'Ramiro Rojo'
  spec.summary = 'Input module'

  spec.add_dependency 'carbuncle-core'
  spec.add_dependency 'carbuncle-math'

  if spec.build.toolchains.include? 'gcc'
    spec.cc.flags << '-std=c11'
    spec.cxx.flags << '-std=c++11'
  end
end
