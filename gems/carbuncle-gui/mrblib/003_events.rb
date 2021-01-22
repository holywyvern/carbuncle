module Carbuncle
  class GUI
    class Events
      def initialize
        @handlers = {}
      end

      def [](name)
        @handlers[name] ||= []
      end

      def add(name, callback)
        self[name].push(callback)
      end

      def remove(name, callback)
        self[name].remove(callback)
      end

      def clear(name)
        @handlers[name] = []
      end

      def call(name)
        self[name].each(&:call)
      end
    end
  end
end
