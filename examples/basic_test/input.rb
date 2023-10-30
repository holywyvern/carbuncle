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
      end
  
      def bind_keyboard(key)
        @key_bindings << key
      end
  
      def bind_gamepad(key, index = -1)
        @gamepad_bindings.push([index, key])
      end
  
      def clear
        @key_bindings.clear
        @gamepad_bindings.clear
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
        return false unless  @gamepad_bindings.any? { |i| gamepad_binding_press?(i) }
        
        controller.last_source = :gamepad
        return true
      end

      def gamepad_binding_press?(gamepad)
        index, key = gamepad
        return true if index < 0 && Carbuncle::Gamepad.any? { |pad| pad.press?(key) }

        Carbuncle::Gamepad[index].press?(key)
      end

      def up?
        keyboard_up? || gamepad_up?
      end

      def keyboard_up?
        Carbuncle::Keyboard.any_up?(*@key_bindings)
      end

      def gamepad_up?
        @gamepad_bindings.any?  { |i| gamepad_binding_up?(i) }
      end

      def gamepad_binding_up?(gamepad)
        index, key = gamepad
        return true if index < 0 && Carbuncle::Gamepad.any? { |pad| pad.up?(key) }

        Carbuncle::Gamepad[index].up?(key)
      end

      def down?
        keyboard_down? || gamepad_down?
      end

      def keyboard_down?
        return false unless Carbuncle::Keyboard.any_down?(*@key_bindings)

        controller.last_source = :keyboard
        return true     
      end

      def gamepad_down?
        return false unless  @gamepad_bindings.any? { |i| gamepad_binding_down?(i) }
        
        controller.last_source = :gamepad
        return true
      end

      def gamepad_binding_down?(gamepad)
        index, key = gamepad
        return true if index < 0 && Carbuncle::Gamepad.any? { |pad| pad.down?(key) }

        Carbuncle::Gamepad[index].down?(key)
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
        return true if index < 0 && Carbuncle::Gamepad.any? { |pad| pad.release?(key) }

        Carbuncle::Gamepad[index].release?(key)
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
