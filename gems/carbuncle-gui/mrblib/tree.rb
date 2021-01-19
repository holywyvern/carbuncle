module Carbuncle
  class GUI
    class Tree < ::Carbuncle::GUI::Container
      attr_accessor :state, :type, :title

      def initialize(parent)
        super
        @state = GUI::MAXIMIZED
        @type = GUI::TREE_NODE
        @title = ''
      end

      def maximized?
        state == GUI::MAXIMIZED
      end

      def minimized?
        state == GUI::MINIMIZED
      end

      def node?
        type == GUI::TREE_NODE
      end

      def tab?
        type == GUI::TREE_TAB
      end

      def update(_dt)
        self.state = gui.tree(title, type, state) do
          super
        end
      end
    end
  end
end
