MRuby::Gem::Specification.new('carbuncle-math') do |spec|
  spec.license = 'MIT'
  spec.author  = 'Ramiro Rojo'
  spec.summary = 'Math module'

  spec.add_dependency 'carbuncle-core'

  if spec.build.toolchains.include? 'gcc'
    spec.cc.flags << '-std=c11'
    spec.cxx.flags << '-std=c++11'
  end
end
