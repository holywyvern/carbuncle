module Carbuncle
  module Vectorizable; end

  # Represents a point in two dimensional space.
  # @note The point allows vectorial operations, with the values of x and y.
  class Point
    include Vectorizable
    extend Vectorizable::ClassMethods

    vectorize_properties %w[x y]

    # @!attribute x [rw]
    #  The x value of the point.
    #  @return [Float]
    # @!attribute y [rw]
    #  The y value of the point.
    #  @return [Float]
    # @!attribute xy [rw]
    #   A vectorial operation to extract [x, y] from the point or to assign it
    #   @return [Carbuncle::Point]
    # @!attribute yx [rw]
    #   A vectorial operation to extract [y, x] from the point or to assign it
    #   @return [Carbuncle::Point]
    #   @example vectorial operations
    #    # swap a value with another:
    #    point.xy = point.yx # swaps x and y
    #    # the same as:
    #    point.x, point.y = point.y, point.x

    def inspect
      "Point(#{x}, #{y})"
    end

    def size
      2
    end

    def to_a
      [x, y]
    end
  end
end
