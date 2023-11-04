include Carbuncle

class ExampleGame < Game
  attr_accessor :text

  def load
    Loader.prepare("pixel-unicode.ttf")
  end

  def start
    Font.default_name = 'pixel-unicode.ttf'
    Font.default_size = 48
    self.text = Text.new
    text.value = 'Hello World from Carbuncle!'
    text.position.set(
      (screen.width - text.width) / 2,
      (screen.height - text.height) / 2
    )
    self << text
  end
end
