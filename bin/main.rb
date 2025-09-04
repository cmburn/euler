class Game < Euler::Game::State
  def load
    @tick = 0
  end

  def update(dt)
    @tick += 1
    if @tick % 1000 == 0
      log.info "FPS: #{system.fps}"
    end
  end

  def input(ev)
    log.info "Received event #{ev.type}"
  end
end

$state = Game.new
