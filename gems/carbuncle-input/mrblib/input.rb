module Carbuncle
  module Input
    class Controller
      attr_accessor :last_source

      def initialize
        self.last_source = :none
        @actions = {}
      end

      def [](action_name)
        @actions[action_name] ||= Carbuncle::Input::Action.new(self)        
      end

      def press?(action)
        self[action].press?
      end

      def down?(action)
        self[action].down?
      end

      def release?(action)
        self[action].release?
      end

      def up?(action)
        self[action].up?
      end

      def clear
        @actions.each_value(&:clear)
      end
    end

    class Action
      attr_reader :controller

      def initialize(controller)
        @controller = controller
        @key_bindings = []
        @gamepad_bindings = []
        @gamepad_axis = []
        @last_axis = 0
      end
  
      def bind_keyboard(key)
        @key_bindings << key
        self
      end

      def key(key)
        bind_keyboard(key)
      end
  
      def bind_gamepad(key, index = -1)
        @gamepad_bindings.push([index, key])
        self
      end

      def gamepad(key, index = -1)
        bind_gamepad(key, index)
      end

      def bind_gamepad_axis(direction, index = -1, positive: true, deadzone: 0.3, axis: :any)
        @gamepad_axis.push({
          direction: direction,
          index: index,
          axis: axis,
          positive: positive,
          deadzone: deadzone
        })
        self
      end

      def axis(direction, index = -1, positive: true, deadzone: 0.3, axis: :any)
        bind_gamepad_axis(direction, index, positive: positive, deadzone: deadzone, axis: axis)
      end
  
      def clear
        @key_bindings.clear
        @gamepad_bindings.clear
        @gamepad_axis.clear
      end
  
      def press?
        keyboard_press? || gamepad_press?
      end

      def keyboard_press?
        return false unless Carbuncle::Keyboard.any_press?(*@key_bindings)

        controller.last_source = :keyboard
        return true     
      end

      def gamepad_press?
        return false unless @gamepad_bindings.any? { |i| gamepad_binding_press?(i) }
        
        controller.last_source = :gamepad
        return true
      end

      def gamepad_binding_press?(gamepad)
        index, key = gamepad
        gamemaps(index).any? { |pad| pad.press?(key) }
      end

      def up?
        keyboard_up? || gamepad_up? || axis_up?
      end

      def keyboard_up?
        Carbuncle::Keyboard.any_up?(*@key_bindings)
      end

      def gamepad_up?
        @gamepad_bindings.any? { |i| gamepad_binding_up?(i) }
      end

      def gamepad_binding_up?(gamepad)
        index, key = gamepad
        gamemaps(index).any? { |pad| pad.up?(key) }
      end

      def axis_up?
        @gamepad_axis.any? do |binding|
          gamepad_axis(binding).any? do |value|
            if binding[:positive]
              value <= binding[:deadzone]
            else
              value >= -binding[:deadzone]
            end
          end
        end
      end

      def down?
        keyboard_down? || gamepad_down? || axis_down?
      end

      def keyboard_down?
        return false unless Carbuncle::Keyboard.any_down?(*@key_bindings)

        controller.last_source = :keyboard
        return true     
      end

      def gamepad_down?
        @gamepad_bindings.any? { |i| gamepad_binding_down?(i) }
      end

      def gamepad_binding_down?(gamepad)
        index, key = gamepad
        gamemaps(index).any? { |pad| pad.down?(key) }
      end

      def axis_down?
        @gamepad_axis.any? do |binding|
          gamepad_axis(binding).any? do |value|
            if binding[:positive]
              value > binding[:deadzone]
            else
              value < -binding[:deadzone]
            end
          end
        end
      end

      def release?
        keyboard_release? || gamepad_release?
      end

      def keyboard_release?
        return false unless Carbuncle::Keyboard.any_release?(*@key_bindings)

        controller.last_source = :keyboard
        return true     
      end

      def gamepad_release?
        return false unless  @gamepad_bindings.any? { |i| gamepad_binding_release?(i) }
        
        controller.last_source = :gamepad
        return true
      end

      def gamepad_binding_release?(gamepad)
        index, key = gamepad
        gamemaps(index).any? { |pad| pad.release?(key) }
      end

      def gamepad_axis(binding)
        return left_axis(binding) + right_axis(binding) if binding[:axis] == :any
        return right_axis(binding) if binding[:axis] == :right

        left_axis(binding)
      end

      def left_axis(binding)
        gamemaps(binding[:index]).map(&:left_axis).map do |point|
          case binding[:direction]
          when :up
            [point.x]
          when :down
            [point.y]
          else
            [point.x, point.y]
          end
        end.flatten
      end

      def right_axis(binding)
        gamemaps(binding[:index]).map(&:right_axis).map do |point|
          case binding[:direction]
          when :up
            [point.x]
          when :down
            [point.y]
          else
            [point.x, point.y]
          end
        end.flatten
      end

      def gamemaps(index)
        return Carbuncle::Gamepad.each if index < 0

        [Carbuncle::Gamepad[index]]
      end
    end
  
    class << self
      def [](controller_name)
        @controllers ||= {}
        @controllers[controller_name] ||= Carbuncle::Input::Controller.new
      end

      def load(file)
        @controllers = {}
      end

      def save(file)
      end

      def player
        @controller ||= self[0]
      end
    end
  end  
end
