module Carbuncle
  class GUI
    class Window < ::Carbuncle::GUI::Container
      attr_reader :title, :rect, :scroll
      attr_writer :id
      attr_accessor :border, :movable, :scalable, :closable, :minimizable,
                    :disable_scrollbar, :show_title, :auto_scrollbar,
                    :background, :scale_left, :disabled, :minimized,
                    :maximized, :visible

      delegate :x, :y, :width, :height, :x=, :y=, :width=, :height, to: :rect

      BOOLEAN_ATTRIBUTES = %i[
        border movable scalable closable minimizable disable_scrollbar
        auto_scrollbar background scale_left disabled
      ].freeze

      def initialize(view)
        unless view.is_a?(::Carbuncle::GUI::Viewport)
          raise ArgumentError, 'Parent must be a Viewport'
        end

        super(view)
        @scroll = Carbuncle::Point.new(0, 0)
        @rect = Carbuncle::Rect.new(0, 0, 1, 1)
        @created = @focused = @hovered = false
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
        update_creation if @created
        gui.window(title, rect, **options) do
          gui.window_scroll = scroll if @created
          @created = true
          super
          update_state
        end
      end

      def update_creation
        gui.set_window_bounds(id, rect)
      end

      def update_state
        rect.set(gui.window_bounds)
        @focused = gui.window_focus?
        @hovered = gui.window_hovered?
        self.scroll = gui.window_scroll
      end

      BOOLEAN_ATTRIBUTES.each do |attr|
        define_method(:"#{attr}?") do
          send(attr)
        end
      end

      def minimized?
        gui.window_minimized?(id)
      end

      def closed?
        gui.window_closed?(id)
      end

      def close
        gui.close_window(id)
      end

      def hidden?
        gui.window_hidden?(id)
      end

      def active?
        gui.window_active?(id)
      end

      def focused?
        @focused
      end

      def hovered?
        @hovered
      end

      def scroll=(value)
        return if value.blank?

        scroll.set(value)
      end

      def show
        gui.show_window(id, true)
      end

      def hide
        gui.show_window(id, false)
      end

      def minimize
        gui.minimize_window(id, true)
      end

      def maximize
        gui.minimize_window(id, false)
      end
    end
  end
end
