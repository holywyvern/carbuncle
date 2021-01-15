  def load
    font = Carbuncle::Font.new('pixel.ttf')
    @text = Carbuncle::Text.new(font)
    @text.value = 'Some text!'
    @text.position.x = 10 # We move the text 10 pixels from the left of the screen.
    @text.position.y = 20 # We move it 20 pixels down from the top of the screen.
    load_textures
    load_background
  end

  # We require to load our images as textures.
  # A texture is an image loaded into our graphics card to draw later.
  def load_textures
    @textures = {
      bg1: Carbuncle::Texture.new('img/bg/parallax-mountain-bg.png'),
      bg2: Carbuncle::Texture.new('img/bg/parallax-mountain-montain-far.png'),
      bg3: Carbuncle::Texture.new('img/bg/parallax-mountain-mountains.png'),
      bg4: Carbuncle::Texture.new('img/bg/parallax-mountain-trees.png'),
      bg5: Carbuncle::Texture.new('img/bg/parallax-mountain-foreground-trees.png')
    }
  end

  # We then are going to load them as Planes.
  # A plane is an image that is repeating forever vertically and horizontally.
  # Because we are going to scroll them forever, this class is perfect for that reason.
  def load_background
    @backgrounds = [
      Carbuncle::Plane.new(@textures[:bg1]),
      Carbuncle::Plane.new(@textures[:bg2]),
      Carbuncle::Plane.new(@textures[:bg3]),
      Carbuncle::Plane.new(@textures[:bg4]),
      Carbuncle::Plane.new(@textures[:bg5])
    ]
    # We scale our backgrounds to double in size. As our screen resolution are twice the size of our images.
    @backgrounds.each { |bg| bg.scale.set(2, 2) }
  end
