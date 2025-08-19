class Game # < Euler::Game::State
  def initialize
    super
#    log.info("Ruby state initialized")
  end
end


$state = Game.new

raise "Failed to init"
