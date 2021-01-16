module Carbuncle
  class GUI
    class Style
      class Color
        def set(*args)
          case args.size
          when 1
            set(*args.to_a)
          when 3
            self.r, self.g, self.b = args[0], args[1], args[2]
          when 4
            self.r, self.g, self.b, self.a = *args
          else
            msg =
              "Wrong number of arguments, expected 1, 3 or 4 but #{args.size} given"
            raise ArgumentError, msg
          end
        end
      end

      def to_a
        [r, g, b, a]
      end
    end
  end
end
