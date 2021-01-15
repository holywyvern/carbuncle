module Carbuncle
  class GUI
    class Widget
      attr_reader :parent

      def initialize(parent)
        @parent = nil
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
    end
  end
end
