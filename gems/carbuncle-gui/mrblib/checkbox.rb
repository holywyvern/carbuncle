module Carbuncle
  class GUI
    class Checkbox < GUI::Widget
      attr_accessor :checked, :label

      def initialize(parent, label = '')
        super(parent)
        self.label = label
        self.checked = false
      end

      def update(_dt)
        self.checked = gui.checkbox(label, checked)
      end
    end
  end
end
