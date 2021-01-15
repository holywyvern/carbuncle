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

  def update(dt)
    update_background(dt)
  end

  def update_background(dt)
    # The first background doesn't move, so we don't update it.
    @backgrounds[1].ox -= dt *  2 # This is a far away mountain, so it moves really slowly.
    @backgrounds[2].ox -= dt *  5 # This mountains are closer, so they move a bit faster.
    @backgrounds[3].ox -= dt * 30 # This trees are closer, so they move even faster
    @backgrounds[4].ox -= dt * 60 # This trees are closes, so they move the fastest
  end

  def draw
    @backgrounds.each(&:draw)
    @text.draw
  end
end
