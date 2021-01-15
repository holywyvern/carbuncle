class TutorialGame < Carbuncle::Game
  def initialize
    super
    # Set our window's title to "Carbuncle Tutorial"
    screen.title = 'Carbuncle Tutorial'
    # Resizes the screen to be 544x320
    screen.resize(544, 320)
  end

  def load
    font = Carbuncle::Font.new('pixel.ttf')
    @text = Carbuncle::Text.new(font)
    @text.value = 'Some text!'
    @text.position.x = 10 # We move the text 10 pixels from the left of the screen.
    @text.position.y = 20 # We move it 20 pixels down from the top of the screen.
  end

  def update(dt)
    # This method is called once per frame, before drawing.
    # dt is the time since the last update call, in seconds.
    # This method is used to update your game logic.
  end

  def draw
    @text.draw
  end
end
