module Carbuncle
  class Tilemap
    def size
      @size ||= Carbuncle::Point.new(width, height).freeze
    end

    def tile_size
      @tile_size ||= Carbuncle::Point.new(tile_width, tile_height).freeze
    end

    def tiles_at(x, y)
      layers
        .select { |l| l.is_a?(Carbuncle::Tilemap::TileLayer) }
        .map { |l| l.tile_at(x, y) }
        .select(&:present?)
    end
  end
end
