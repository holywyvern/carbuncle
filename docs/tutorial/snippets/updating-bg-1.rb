  def update(dt)
    update_background(dt)
  end

  def update_background(dt)
    @backgrounds.each do |bg|
      # We will make them move 50 pixels per second.
      # We multiple our update time (dt) in seconds per 50 pixels.
      bg.ox -= 50 * dt
    end
  end

  def draw
    @backgrounds.each(&:draw)
    @text.draw
  end
