def load
  font = Carbuncle::Font.new('pixel.ttf')
  @text = Carbuncle::Text.new(font)
  @text.value = 'Some text!'
  @text.position.x = 10 # We move the text 10 pixels from the left of the screen.
  @text.position.y = 20 # We move it 20 pixels down from the top of the screen.
end
