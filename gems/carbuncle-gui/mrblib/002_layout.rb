module Carbuncle
  class GUI
    class Layout
      def render(gui)
        yield if block_given?
      end

      def apply(_widget); end
    end
  end
end
