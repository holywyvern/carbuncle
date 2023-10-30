include Carbuncle

class ExampleGame < Game
  attr_accessor :label, :sprites, :selected, :labels, :help

  def load
    Loader.prepare("pixel-unicode.ttf")
  end

  def start
    Font.default_name = 'pixel-unicode.ttf'
    self.sprites = []
    self.labels = []
    self.label = Text.new
    self.help = Text.new
    self.help.value = 'Click to change generated image'
    self.help.position.set(
      (screen.width - help.width) / 2,
      screen.height - help.height
    )
    self.label.position.set(
      5, 5
    )
    label.color.set(255, 0, 0)
    help.color.set(255, 0, 0)
    self.selected = 0
    generate_textures
    change_selected
  end

  def generate_textures
    c1 = Color.new(0, 0, 0)
    c2 = Color.new(255, 255, 255)
    sprites << make_sprite(Bitmap.color(640, 480, c1))
    labels << "Color"
    sprites << make_sprite(Bitmap.linear_gradient(640, 480, c1, c2))
    labels << "Horizontal Gradient"
    sprites << make_sprite(Bitmap.linear_gradient(640, 480, c1, c2, true))
    labels << "Vertical Gradient"
    sprites << make_sprite(Bitmap.radial_gradient(640, 480, 0.5, c1, c2))
    labels << "Radial Gradient"
    sprites << make_sprite(Bitmap.checked(640, 480, 32, 32, c1, c2))
    labels << "Checked"
    sprites << make_sprite(Bitmap.white_noise(640, 480, 0.1))
    labels << "White Noise"
    sprites << make_sprite(Bitmap.perlin_noise(640, 480, 0, 0, 1))
    labels << "Perlin Noise"
    sprites << make_sprite(Bitmap.cellular(640, 480, 64))
    labels << "Cellular"
  end

  def make_sprite(bmp)
    texture = Texture.new(bmp)
    bmp.dispose
    Sprite.new(texture)
  end

  def update(dt)
    change_selected if Mouse.press?(:left) || Mouse.press?(:right)
  end

  def change_selected
    self.selected = (self.selected + 1) % sprites.size
    self.label.value = labels[selected]
  end

  def draw
    sprites[selected].draw
    help.draw
    label.draw
  end
end
