class Game::Player < Carbuncle::Sprite
  attr_reader :frame, :pose, :frame_width, :frame_height, :animation_step

  def initialize
    super(Carbuncle::Texture.new('graphics/characters/kurea.png'))
    setup_actions    
    @sound = Sound.new('sound/coin.wav')
    @frame_width = texture.width / 4
    @frame_height = texture.height / 10
    @frame = 0
    @pose = 0
    @animation_time = 0
    @animation_step = (1.0 / 60.0) * 8
    pivot.set(0.5, 1)
    @attacking = false
    update_rect
  end

  def setup_actions
    @actions = Carbuncle::Input.player
    setup_attack
    setup_left
    setup_right
  end

  def setup_attack
    @attack_action = @actions[:attack]
    @attack_action.bind_keyboard(:a)
    @attack_action.bind_gamepad(:RIGHT_FACE_DOWN)      
  end

  def setup_left
    @left_action = @actions[:left]
    @left_action.bind_keyboard(:left)
    @left_action.bind_gamepad(:DPAD_LEFT)
    @left_action.bind_gamepad_axis(:x, positive: false) 
  end

  def setup_right
    @right_action = @actions[:right]
    @right_action.bind_keyboard(:right)
    @right_action.bind_gamepad(:DPAD_RIGHT)
    @right_action.bind_gamepad_axis(:x, positive: true)
  end

  def update(dt)
    update_input(dt)
    update_frame(dt)
  end

  def update_input(dt)
    return if attacking?

    dx = 0
    if @attack_action.press?
      attack!
    elsif @left_action.down?
      dx -= 80
      scale.x = 1
      @frame = 0 if @pose != 4
      @pose = 4
    elsif @right_action.down?
      dx += 80
      @frame = 0 if @pose != 5
      @pose = 5
    else
      @frame = 0 if @pose != 0
      @pose = 0
    end
    position.x += dx * dt
  end

  def update_frame(dt)
    @animation_time += dt
    while @animation_time > animation_step
      @animation_time -= animation_step
      @frame = (@frame + 1) % 4
      if attacking? && @frame == 0
        @attacking = false
        @frame = 0
      end
      update_rect
    end
  end

  def update_rect
    src_rect.set(frame * frame_width, pose * frame_height, frame_width, frame_height)
  end

  def attacking?
    @attacking
  end

  def attack!
    @sound.play
    @attacking = true
    @pose = rand > 0.5 ? 6 : 7
    @frame = 0
  end
end
