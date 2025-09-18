/* SPDX-License-Identifier: ISC */

#include "euler/game/system.h"

#include "euler/game/state.h"

euler::game::System::System(const util::Reference<State> &state)
    : _state(state)
{
	_tick = SDL_GetTicks();
	_last_tick = _tick;
}

euler::util::Reference<euler::game::State>
euler::game::System::state() const
{
	return _state.strengthen();
}
