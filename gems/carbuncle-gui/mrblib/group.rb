module Carbuncle
  class GUI
    class Group < ::Carbuncle::GUI::Container
      attr_reader :title
      attr_writer :id
      attr_accessor :border, :movable, :scalable, :closable, :minimizable,
                    :disable_scrollbar, :show_title, :auto_scrollbar,
                    :background, :scale_left, :disabled, :minimized,
                    :maximized, :visible

      def initialize(view)
        super(view)
        self.title = ''
        self.id = nil
        initialize_flags
      end

      def initialize_flags
        self.border = self.show_title = self.visible = true
        self.movable = self.scalable = self.closable = false
        self.minimizable = self.disable_scrollbar = false
        self.auto_scrollbar = true
        self.background = self.scale_left = self.disabled = false
      end

      def id
        @id || title
      end

      def title=(value)
        @title = (value || '').to_s
      end

      def options
        {
          id: id,
          border: border, movable: movable, scalable: scalable,
          closable: closable, minimizable: minimizable,
          disable_scrollbar: disable_scrollbar,
          title: show_title,
          auto_scrollbar: auto_scrollbar, background: background,
          scale_left: scale_left, disable: disabled,
          minimized: minimized, maximized: maximized
        }
      end

      def update(_dt)
        gui.group(title, **options) do
          super
        end
      end

      def border?
        border
      end

      def movable?
        movable
      end

      def scalable?
        scalable
      end

      def closable?
        closable
      end

      def minimizable?
        minimizable
      end

      def disable_scrollbar?
        disable_scrollbar
      end

      def auto_scrollbar?
        auto_scrollbar
      end

      def background?
        background
      end

      def scale_left?
        scale_left
      end

      def disabled?
        disabled
      end
    end
  end
end
