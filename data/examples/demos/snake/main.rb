include Carbuncle

TILE_SIZE = 32

BLACK = Color.new(0, 0, 0)
WHITE = Color.new(255, 255, 255)
RED = Color.new(255, 0, 0)

OPPOSITES = {
  up: :down,
  down: :up,
  left: :right,
  right: :left
}

class Snake
  attr_accessor :segments, :speed, :score, :texture, :game, :opacity

  delegate :tiles_x, :tiles_y, to: :game
  delegate :direction, to: :head

  def initialize(game, texture)
    self.game = game
    self.texture = texture
    self.segments = [Snake::Head.new(self, texture)]
    self.speed = 8
    self.score = 0
    self.opacity = 0
  end

  def head
    segments.first
  end

  def tail
    segments.last
  end

  def draw
    segments.reverse_each(&:draw)
  end

  def update(dt)
    segments.reverse_each { |i| i.update(dt) }
    update_keyboard
  end

  def update_keyboard
    %i[up down left right].each do |k|
      next if head.opposite?(k)

      head.next_direction = k if Keyboard.down?(k)
    end
  end

  def eat
    head.full = true
    self.score += 1
    self.speed += 1 if speed < 16
  end

  def add_segment
    tail.next_segment = Snake::Segment.new(self, texture)
    tail.next_segment.move_to(tail.current)
    tail.next_segment.direction = tail.direction
    tail.next_segment.target.set(tail.target)
    segments.push(tail.next_segment)
  end
end

class Snake::Segment < Sprite
  attr_accessor :snake, :target, :current, :next_segment,
                :real_position, :direction, :next_direction,
                :full, :next_segment

  delegate :speed, :game, to: :snake
  delegate :tiles_x, :tiles_y, to: :game

  def initialize(snake, texture)
    super(texture)
    self.snake = snake
    src_rect.set(0, width / 2, width / 2, height / 2)
    pivot.set(0.5, 0.5)
    self.target = Point.new(1, 1)
    self.current = target.dup
    self.real_position = Point.new(TILE_SIZE, TILE_SIZE)
    self.direction = :right
    self.next_direction = :right
    self.full = false
    self.next_segment = nil
  end

  def update(dt)
    if moving?
      update_movement(dt)
    else
      update_food
    end
    update_sprite
  end

  def update_food
    if full
      self.full = false
      snake.score += 1
      if next_segment
        next_segment.full = true
      else
        snake.add_segment
      end
    end
  end

  def move_to(position)
    self.current.set(position)
    self.real_position.set(
      position.x * TILE_SIZE,
      position.y * TILE_SIZE
    )
  end

  def moving?
    target_x != real_position.x.floor || target_y != real_position.y.floor
  end

  def target_x
    target.x * TILE_SIZE
  end

  def target_y
    target.y * TILE_SIZE
  end

  def current_x
    real_position.x.floor / TILE_SIZE
  end

  def current_y
    real_position.y.floor / TILE_SIZE
  end

  def update_direction(prev)
    next_segment.update_direction(self) if next_segment
    self.current.set(prev.current)
    self.target.set(prev.target)
  end

  def update_movement(dt)
    dx = (target.x - current_x) * dt * speed * TILE_SIZE
    dy = (target.y - current_y) * dt * speed * TILE_SIZE
    real_position.x += dx
    real_position.y += dy
  end

  def update_sprite
    position.x = real_position.x.floor + TILE_SIZE / 2
    position.y = real_position.y.floor + TILE_SIZE / 2
    self.angle = calculate_angle
    self.opacity = Math.cos(snake.opacity * 4 * Math::PI) * 255
    update_rect
  end

  def update_rect
    if full
      src_rect.x = 0
    else
      src_rect.x = width
    end
  end

  def calculate_angle
    return 90 if direction == :right
    return -90 if direction == :left
    return 180 if direction == :down

    0
  end

  def opposite?(dir)
    OPPOSITES[dir] == direction
  end
end

class Snake::Head < Snake::Segment
  delegate :segments, to: :snake

  def initialize(snake, texture)
    super(snake, texture)
    src_rect.y = 0
  end

  def update_rect
  end

  def update(dt)
    if moving?
      update_movement(dt)
    else
      if game_over?
        game.state = :game_over
        update_opacity(dt)
      else
        update_food
        update_direction
      end
    end
    update_sprite
  end

  def game_over?
    touch_edge? || bitten?
  end

  def touch_edge?
    target.x <= 0 || target.y <= 0 ||
    target.x - 1 >= tiles_x || target.y - 1 >= tiles_y
  end

  def bitten?
    segments.any? do |segment|
      segment != self && segment.current == self.current
    end
  end

  def update_opacity(dt)
    snake.opacity += dt
  end

  def update_direction
    next_segment.update_direction(self) if next_segment
    current.set(target)
    case next_direction
    when :up
      target.y -= 1
    when :down
      target.y += 1
    when :left
      target.x -= 1
    when :right
      target.x += 1
    end
    self.direction = next_direction
  end
end

class Background < Sprite
  def initialize(game)
    super(create_texture(game))
  end

  def create_texture(game)
    width = game.screen.width
    height = game.screen.height
    bmp = Bitmap.checked(width, height, TILE_SIZE, TILE_SIZE, BLACK, WHITE)
    bmp.stroke_rect(Rect.new(0, 0, width, height), RED, TILE_SIZE)
    Texture.new(bmp).tap { |_| bmp.dispose }
  end
end

class Fruit < Sprite
  attr_accessor :game, :current

  delegate :snake, :tiles_x, :tiles_y, to: :game

  def initialize(game, texture)
    super(texture)
    self.game = game
    self.current = Point.new
    src_rect.set(TILE_SIZE, 0, TILE_SIZE, TILE_SIZE)
    relocate
  end

  def update(dt)
    if snake.head.current == current
      snake.eat
      relocate
    end
    update_position
  end

  def relocate
    current.set(
      rand(tiles_x) + 1,
      rand(tiles_y) + 1
    )
    relocate if snake.segments.any? { |i| i.current == current }
  end

  def update_position
    position.set(
      current.x * TILE_SIZE,
      current.y * TILE_SIZE
    )
  end
end

class SnameGame < Game
  attr_accessor :texture, :snake, :background, :fruit, :state, :title, :game_over, :score

  def load
    self.state = :title
    self.texture = Texture.new('snake.png')
    self.background = Background.new(self)
    self.score = Text.new
    score.x = 10
    create_title
    create_game_over
    reset
  end

  def create_title
    self.title = Text.new
    title.value = 'Press ENTER to start'
    title.position.set(
      (screen.width - title.width) / 2,
      (screen.height - title.height) / 2
    )
    title.color.set(0, 0, 255)
  end

  def create_game_over
    self.game_over = Text.new
    game_over.value = 'Game Over - Press ENTER to restart'
    game_over.position.set(
      (screen.width - game_over.width) / 2,
      (screen.height - game_over.height) / 2
    )
    game_over.color.set(255, 0, 0)
  end

  def reset
    game_over.color.alpha = 0
    self.snake = Snake.new(self, texture)
    self.fruit = Fruit.new(self, texture)
  end

  def tiles_x
    @tiles_x ||= screen.width / TILE_SIZE - 2
  end

  def tiles_y
    @tiles_y ||= screen.height / TILE_SIZE - 2
  end

  def update(dt)
    case state
    when :title
      update_title(dt)
    when :play
      snake.update(dt)
      fruit.update(dt)
    when :game_over
      game_over.color.alpha = 255
      snake.update(dt)
      if Keyboard.press?(:enter)
        reset 
        self.state = :play
      end
    end
    score.value = "Score: #{snake.score}"
  end

  def update_title(dt)
    self.state = :play if Keyboard.press?(:enter)

    snake.segments.each(&:update_sprite)
    fruit.update(dt)
  end

  def draw
    background.draw
    fruit.draw
    snake.draw
    if state == :title
      title.draw
    end
    if state == :game_over
      game_over.draw
    end
    score.draw
  end
end
