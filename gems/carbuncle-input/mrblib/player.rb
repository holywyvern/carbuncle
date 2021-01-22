module Carbuncle
  class Player
    attr_reader :actions

    class Binding
      attr_accessor :origin, :key, :gamepad_index

      def initialize(key:, origin: 'keyboard', gamepad_index: 0)
        @origin = origin
        @key = key
        @gamepad_index = gamepad_index
      end

      def press?
        case origin
        when 'keyboard'
          Keyboard.press?(key)
        when 'gamepad'
          Gamepad[gamepad_index].press?(key)
        end
      end

      def up?
        case origin
        when 'keyboard'
          Keyboard.up?(key)
        when 'gamepad'
          Gamepad[gamepad_index].up?(key)
        end
      end

      def down?
        case origin
        when 'keyboard'
          Keyboard.down?(key)
        when 'gamepad'
          Gamepad[gamepad_index].down?(key)
        end
      end

      def release?
        case origin
        when 'keyboard'
          Keyboard.release?(key)
        when 'gamepad'
          Gamepad[gamepad_index].release?(key)
        end
      end
    end

    def initialize
      @actions = {}
    end

    def press?(action)
      actions = @actions[action]
      return false if actions.blank?
      return actions.any?(&:press?) if actions.is_a?(Array)

      actions.press?
    end

    def up?(action)
      actions = @actions[action]
      return false if actions.blank?
      return actions.any?(&:up?) if actions.is_a?(Array)

      actions.up?
    end

    def down?(action)
      actions = @actions[action]
      return false if actions.blank?
      return actions.any?(&:down?) if actions.is_a?(Array)

      actions.down?
    end

    def release?(action)
      actions = @actions[action]
      return false if actions.blank?
      return actions.any?(&:release?) if actions.is_a?(Array)

      actions.release?
    end
  end
end
