module Carbuncle
  class GUI
    class Button < GUI::Widget
      attr_accessor :label

      def initialize(parent, label = '')
        super
        self.label = label
      end

      def update(_dt)
        events.call('click') if gui.button(label)
      end
    end
  end
end
