module Carbuncle
  class Game
    class << self
      # @!attribute [r] current_game
      #   The current game, that's already running.
      #   @return [Carbuncle::Game]
      # @!method running?
      #   Returns true if a game is running
      #   @return [Boolean]
    end

    # @!group Lifecycle methods

    # This method is called before the graphics context are created.
    # Only really used when working on the web.
    # This will prepare the files on the filesystem so the game can load
    # them properly.
    # Any file preloaded here will be awaited before starting the engine, on the loading screen.
    # Any other initial configuration like screen size would be nice to be added here.
    # If you don't preload the file here, it will still be loaded, but may hang the
    # website if the file is too big.
    # @example The game is a web game
    #   class MyGame < Carbuncle::Game
    #     def load
    #       Loader.prepare("img/hero.png")
    #     end
    #
    #     def start
    #        @hero = Texture.new("img/hero.png")
    #        # the rest of your code here...
    #     end
    #   end
    # @return [nil]
    def load; end

    # This method is called after the graphics context is created and assets were prepared.
    # This method is the one you run to setup the game.
    # @example The game starts
    #   class MyGame < Carbuncle::Game
    #     def start
    #       # The assets are loaded, so we can load our assets
    #       SceneManager.set(TitleScene)
    #     end
    #   end
    # @return [nil]
    def start; end

    # This method is called after an update, usually once per frame.
    # If the application skips frames, this method won't be called.
    # @param [Float] dt The time since the last update.
    # @return [nil]
    def update(dt); end

    # This method is called once per frame. This is inside the drawing context,
    # so you can only draw objects inside this function.
    # @return [nil]
    def draw; end

    # @!group Lifecycle events

    # This event is called before the game starts, even before the screen is initialized.
    # If you require to load your assets, use Carbuncle::Game#load instead.
    # @return [nil]
    def before_run; end

    # This event is called after the game starts, even after load.
    # You can use this event to trigger your start, if you want to keep it separated from load.
    # @return [nil]
    def after_run; end

    # This event is called before the game is closed, useful for cleanup.
    # @return [nil]
    def before_close; end

    # This event is called after the game is closed and the screen is destroyed.
    # @return [nil]
    def after_close; end

    # This method is called when a file is dropped.
    # @param [Array] files A list of files dropped into the window.
    # @return [nil]
    def file_dropped(*files); end
  end
end
