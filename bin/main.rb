class Game < Euler::Game::State
  def initialize
    super
    log.info "Hello from Ruby!"
  end
end


$state = Game.new

