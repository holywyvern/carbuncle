module Carbuncle
  class GUI
    class Chart < Carbuncle::GUI::Widget
      attr_reader :values
      attr_accessor :type, :max, :min, :color, :highlight_color

      delegate :size, to: :values

      def initialize(parent)
        super
        @values = []
        @type = :line
        @max = @min = nil
        @color = Carbuncle::Color.new(100, 100, 100)
        @highlight_color = @color + 50
      end

      def update(dt)
        return if values.empty?

        chart = :"#{type}_chart"
        gui.send(chart, color, highlight_color, size, min_value, max_value) do
          values.each do |value|
            gui.chart_push(value)
          end
        end
      end

      def max_value
        max || (values.max + 1)
      end

      def min_value
        min || (values.min - 1)
      end
    end
  end
end
