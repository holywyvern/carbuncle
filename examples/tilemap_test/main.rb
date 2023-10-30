include Carbuncle

class TestGame < Carbuncle::Game
  def load
    load_map
  end

  def load_map
    @camera = Carbuncle::Camera.new
    @camera.scale = 0.5
    @camera.offset.set(0, 128 * -3.7)
    @map = Carbuncle::Tilemap.new('graphics/maps/example.tmx')
  end

  def update(dt)  
    take_screenshot if Keyboard.press?(:s)
  end

  def take_screenshot
    Bitmap.screenshot.save('screen.png')
  end

  def draw
    
    @camera.draw do
      @map.draw
    end
  end
end
