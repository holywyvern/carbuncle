class TutorialGame < Carbuncle::Game
  def initialize
    super
    # You can set up your screen size and title here
  end

  def load
    # This method is called when your screen is created,
    # so you can load any asset, even textures now.
    # Boot your game from here.
  end

  def update(dt)
    # This method is called once per frame, before drawing.
    # dt is the time since the last update call, in seconds.
    # This method is used to update your game logic.
  end

  def draw
    # This is called after each update, and while the screen is able to be rendered.
    # You should draw your characters and backgrounds here.
  end
end
