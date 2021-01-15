module Carbuncle
  class Text
    delegate :x, :y, :x=, :y=, to: :position

    def update(dt); end

    def size
      font.measure_text(value)
    end

    def width
      size.x
    end

    def height
      size.y
    end

    def rect
      s = size
      Carbuncle::Rect.new(x, y, s.x, s.y)
    end
  end
end
