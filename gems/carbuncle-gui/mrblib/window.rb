module Carbuncle
  class GUI
    class Window < ::Carbuncle::GUI::Container
      attr_reader :title, :rect
      attr_writer :id
      attr_accessor :border, :movable, :scalable, :closable, :minimizable,
                    :disable_scrollbar, :show_title, :auto_scrollbar,
                    :background, :scale_left, :disabled, :minimized,
                    :maximized, :visible

      delegate :gui, to: :parent

      def initialize(view)
        unless view.is_a?(::Carbuncle::GUI::Viewport)
          raise ArgumentError, 'Parent must be a Viewport'
        end

        super(view)
        @rect = Carbuncle::Rect.new(0, 0, 1, 1)
        self.title = ''
        self.id = nil
        initialize_flags
      end

      def initialize_flags
        self.border = self.show_title = self.visible = true
        self.movable = false
        self.scalable = false
        self.closable = false
        self.minimizable = false
        self.disable_scrollbar = false
        self.auto_scrollbar = true
        self.background = false
        self.scale_left = false
        self.disabled = false
      end

      def id
        @id || title
      end

      def title=(value)
        @title = (value || '').to_s
      end

      def rect=(value)
        rect.set(value) if value.present?
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
        return unless visible

        self.visible = gui.window(title, rect, **options) do
          super
        end
      end
    end
  end
end
