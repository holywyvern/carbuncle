  def update_background(dt)
    # The first background doesn't move, so we don't update it.
    @backgrounds[1].ox -= dt *  2 # This is a far away mountain, so it moves really slowly.
    @backgrounds[2].ox -= dt *  5 # This mountains are closer, so they move a bit faster.
    @backgrounds[3].ox -= dt * 30 # This trees are closer, so they move even faster
    @backgrounds[4].ox -= dt * 60 # This trees are closes, so they move the fastest
  end