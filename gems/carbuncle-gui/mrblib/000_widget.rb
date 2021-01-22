module Carbuncle
  class GUI
    class Widget
      attr_reader :parent, :rect, :events

      delegate :gui, :layout, to: :parent
      delegate :x, :x=, :y, :y=, :width, :width=, :height, :height=,
               to: :rect

      def initialize(parent)
        @parent = nil
        @rect = Carbuncle::Rect.new
        @events = Events.new
        self.parent = parent
      end

      def parent=(value)
        return if parent == value

        if parent.present?
          old_parent = parent
          @parent = nil
          old_parent.remove_child(self)
        end
        return if value.blank?

        @parent = value
        @parent.add_child(self)
      end

      def update(dt); end

      def remove
        return if parent.blank?

        parent.remove_child(self)
      end

      def rect=(value)
        return if value.blank?

        rect.set(value)
      end
    end
  end
end
