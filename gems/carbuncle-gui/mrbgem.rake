MRuby::Gem::Specification.new('carbuncle-gui') do |spec|
  spec.license = 'MIT'
  spec.author  = 'Ramiro Rojo'
  spec.summary = 'GUI module (based on raygui)'

  spec.add_dependency 'carbuncle-core'
  spec.add_dependency 'carbuncle-graphics'

  if spec.build.toolchains.include? 'gcc'
    spec.cc.flags << '-std=c11'
    spec.cxx.flags << '-std=c++11'
  end
end
