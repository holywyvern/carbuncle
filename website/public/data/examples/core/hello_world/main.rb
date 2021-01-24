include Carbuncle

class ExampleGame < Game
  attr_accessor :text

  def load
    self.text = Text.new
    text.value = 'Hello World from Carbuncle!'
    text.position.set(
      (screen.width - text.width) / 2,
      (screen.height - text.height) / 2
    )
  end

  def draw
    @text.draw
  end
end
