module Carbuncle
  class GUI
    class ComboBox < GUI::Widget
      attr_reader :items
      attr_accessor :title, :selected, :size

      def initialize(parent)
        super
        @items = []
        @size = Carbuncle::Point.new(0, 0)
        @selected = 0
      end

      def update(_dt)
        gui.combo(selected_symbol, size) do
          items.each_with_index do |item, index|
            @selected = index if item.draw(gui)
          end
        end
      end

      def selected_symbol
        selected_item&.title || ''
      end

      def selected_item
        items[selected]
      end

      def add_item(title)
        item = GUI::ComboBox::Item.new
        item.title = title
        items.push(item)
      end

      class Item
        attr_accessor :title

        def initialize
          self.title = ''
        end

        def draw(gui)
          gui.combo_item_text(title)
        end
      end
    end
  end
end
