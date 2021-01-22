module Carbuncle
  class GUI
    class ColorPicker < GUI::Widget
      attr_accessor :color, :format

      def initialize(parent)
        super
        self.color = Color.new(255, 0, 0)
        self.format = GUI::RGBA
      end

      def update(_dt)
        gui.color_picker(color, format)
      end
    end
  end
end
