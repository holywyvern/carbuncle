module Carbuncle
  module Vectorizable; end

  # Represents a point in three dimensional space.
  class Vector3
    include Vectorizable
    extend Vectorizable::ClassMethods

    vectorize_properties %w[x y z]

    # @!attribute x [rw]
    #  The x value of the vector.
    #  @return [Float]
    # @!attribute y [rw]
    #  The y value of the vector.
    #  @return [Float]
    # @!attribute z [rw]
    #  The z value of the vector.
    #  @return [Float]
    # @!attribute xy [rw]
    #   @return [Carbuncle::Point]
    # @!attribute xz [rw]
    #   @return [Carbuncle::Point]
    # @!attribute yx [rw]
    #   @return [Carbuncle::Point]
    # @!attribute yz [rw]
    #   @return [Carbuncle::Point]
    # @!attribute zx [rw]
    #   @return [Carbuncle::Point]
    # @!attribute zy [rw]
    #   @return [Carbuncle::Point]
    # @!attribute xyz [rw]
    #   @return [Carbuncle::Vector3]
    # @!attribute xzy [rw]
    #   @return [Carbuncle::Vector3]
    # @!attribute yxz [rw]
    #   @return [Carbuncle::Vector3]
    # @!attribute yzx [rw]
    #   @return [Carbuncle::Vector3]
    # @!attribute zxy [rw]
    #   @return [Carbuncle::Vector3]
    # @!attribute xzy [rw]
    #   @return [Carbuncle::Vector3]

    def inspect
      "Vector3(#{x}, #{y}, #{z})"
    end

    def size
      3
    end

    def to_a
      [x, y, z]
    end
  end
end
