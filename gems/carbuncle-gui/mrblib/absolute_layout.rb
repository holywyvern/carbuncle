module Carbuncle
  class GUI
    class AbsoluteLayout < Carbuncle::GUI::Layout
      attr_accessor :dynamic, :height

      def dynamic?
        dynamic
      end

      def render(container, &block)
        h = height || container.rect.height
        container.gui.space_layout(dynamic?, h, container.size) do
          super(container, &block)
        end
      end

      def apply(widget)
        widget.gui.space_push(widget.rect)
      end
    end
  end
end
