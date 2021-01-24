import Example from "../example";

const example1 = `
include Carbuncle

class Example < Game
  def load
    @text = Text.new
    @text.value = 'Hello World'
  end

  def draw
    @text.draw
  end
end
`.trim();

const example2 = `
include Carbuncle

class Example < Game
  def load
    texture = Texture.new('img/carbuncle.png')
    @sprite = Sprite.new(texture)
    @sprite.position.set(center)
    @sprite.pivot.set(0.5, 0.5)
  end

  def draw
    @sprite.draw
  end

  def center
    Point.new(
      screen.width / 2,
      screen.height / 2
    )
  end
end
`.trim();

const example3 = `
include Carbuncle

class Example < Game
  def load
    @music = Music.new('bgm/awesome.ogg')
    @sound = Sound.new('sfx/click.wav')
    @music.play
  end

  def update(dt)
    @sound.play if Keyboard.press?(:space)
  end
end
`.trim();

function Examples() {
  return (
    <>
      <h3>Quick examples</h3>
      <Example title="Hello World">
        {example1}        
      </Example>
      <Example title="Drawing an Image">
        {example2}
      </Example>
      <Example title="Playing music and sound">
        {example3}
      </Example>
    </>
  )
}

export default Examples;