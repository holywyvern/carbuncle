module Carbuncle
  module Vectorizable; end
  # Represents a rectangle
  class Rect
    include Vectorizable
    extend Vectorizable::ClassMethods

    vectorize_properties %w[x y w h]

    def inspect
      "Rect(#{x}, #{y}, #{w}, #{h})"
    end

    def size
      4
    end

    def to_a
      [x, y, w, h]
    end
  end
end
