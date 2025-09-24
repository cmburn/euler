class Game < Euler::Game::State
  WINDOW_RECT = {x: 50, y: 50, w: 220, h: 220}
  WINDOW_FLAGS = %I[border moveable closeable]

  # 93825003446896

  def load
    @tick = 0
    @time = 0.0
  end

  def update(dt)
    @time += dt
    # log.info("#{@tick}")
    if @time >= 1.0
      @time = 0.0
      log.info "FPS: #{system.fps.round(3)}"
    end
  end

  def input(ev)
    log.info "Received event #{ev.type}"
  end

  def draw
    # gui.window("Title", WINDOW_RECT, WINDOW_FLAGS) do |win|
    #   win.row(:dynamic, height: 30, columns: 2) do
    #     gui.button(label: 'Press me!') do
    #       log.info "Button pressed!"
    #     end
    #   end
    # end
  end

  def quit
    log.info "Received quit notification"
  end
end

$state = Game.new
