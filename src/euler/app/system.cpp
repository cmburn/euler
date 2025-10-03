/* SPDX-License-Identifier: ISC */

#include "euler/app/system.h"

#include "euler/app/state.h"

euler::app::System::System(const util::Reference<State> &state)
    : _state(state)
{
	_tick = SDL_GetTicks();
	_last_tick = _tick;
}

euler::app::System::~System()
{
	assert(false);
}

euler::util::Reference<euler::app::State>
euler::app::System::state() const
{
	return _state.strengthen();
}

void
euler::app::System::tick()
{
	_last_tick = _tick;
	_tick = SDL_GetTicks();
	++_total;
	if (_tick - _last_frames_tick < 1000) return;
	const float frames = _total - _last_frames_total;
	const float seconds = (_tick - _last_frames_tick) / 1000.0f;
	_fps = frames / seconds;
	_last_frames_tick = _tick;
	_last_frames_total = _total;
}
