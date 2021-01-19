module Carbuncle
  class GUI
    class Container < ::Carbuncle::GUI::Widget
      attr_reader :layout

      def initialize(parent)
        super
        @children = []
        @layout = Carbuncle::GUI::DynamicLayout.new
      end

      def size
        @children.size
      end

      def add_child(child)
        return if @children.include?(child)

        @children.push(child)
        child.parent = self
      end

      def remove_child(child)
        return unless @children.include?(child)

        @children.delete(child)
        child.parent = nil
        self
      end

      def children
        @children.dup
      end

      def layout=(value)
        is_layout = value.is_a?(Carbuncle::GUI::Layout)
        @layout =
          is_layout ? value : Carbuncle::GUI::DynamicLayout.new
      end

      def update(dt)
        super
        layout.render(self) do
          @children.each do |c|
            layout.apply(c)
            c.update(dt)
          end
        end
      end
    end
  end
end
