class Carbuncle::Color
  include Carbuncle::Vectorizable
  extend Carbuncle::Vectorizable::ClassMethods

  vectorize_properties %w[r g b a]

  class << self
    def random
      new(rand(255), rand(255), rand(255))
    end
  end

  def inspect
    "Color(#{r}, #{g}, #{b}, #{a})"
  end

  def size
    4
  end

  def to_a
    [r, g, b, a]
  end
end
