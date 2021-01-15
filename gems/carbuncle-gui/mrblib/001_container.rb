module Carbuncle
  class GUI
    class Container < ::Carbuncle::GUI::Widget
      def initialize(parent)
        super
        @children = []
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

      def update(dt)
        @children.each { |c| c.update(dt) }
      end
    end
  end
end
