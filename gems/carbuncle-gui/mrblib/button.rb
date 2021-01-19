module Carbuncle
  class GUI
    class Button < GUI::Widget
      attr_accessor :label, :align
      attr_reader :texture

      def initialize(parent, label = '')
        super(parent)
        self.label = label
        self.align = Style::Align::MIDDLE|Style::Align::CENTERED
        self.texture = nil
      end

      def texture=(value)
        @texture = value
        rect.set(0, 0, value.width, value.height) if value.present?
      end

      def update(_dt)
        events.call('click') if update_button
      end

      private

      def update_button
        return update_image_button if texture.present?

        gui.button(label)
      end

      def update_image_button
        return gui.image_button(texture, rect, label, align) if label.present?

        gui.image_button(texture, rect)
      end
    end
  end
end
