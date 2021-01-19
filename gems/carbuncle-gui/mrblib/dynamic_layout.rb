module Carbuncle
  class GUI
    class DynamicLayout < Layout
      attr_accessor :height, :rows

      def initialize
        super
        self.height = 24
        self.rows = 1
      end

      def render(container)
        container.gui.dynamic_layout(height, rows)
        super
      end
    end
  end
end
