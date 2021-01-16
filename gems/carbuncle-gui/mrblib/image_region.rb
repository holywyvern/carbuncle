module Carbuncle
  class GUI
    class Style
      class Image
        class Region
          def x
            self[0]
          end

          def y
            self[1]
          end

          def w
            self[2]
          end

          def h
            self[3]
          end

          def width
            self[2]
          end

          def height
            self[3]
          end

          def x=(value)
            self[0] = value
          end

          def y=(value)
            self[1] = value
          end

          def w=(value)
            self[2] = value
          end

          def h=(value)
            self[3] = value
          end

          def width=(value)
            self[2] = value
          end

          def height=(value)
            self[3] = value
          end

          def to_a
            [x, y, w, h]
          end
        end
      end
    end
  end
end
