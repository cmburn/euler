/* SPDX-License-Identifier: ISC */

#ifndef EULER_GAME_SYSTEM_H
#define EULER_GAME_SYSTEM_H

#include <SDL3/SDL_timer.h>

#include "euler/util/object.h"

namespace euler::game {
class State;

/*
 * Primarily responsible for miscellaneous SDL and timing functions.
 * Only separated from State for clarity.
 */
class System final : public util::Object {
	friend class State;
public:
	explicit System(const util::Reference<State> &state = nullptr);
	util::Reference<State> state() const;

	[[nodiscard]] float
	dt() const
	{
		return (_tick - _last_tick) / 1000.0f;
	}

	[[nodiscard]] int16_t
	fps() const
	{
		return _fps;
	}

	void
	tick()
	{
		_last_tick = _tick;
		_tick = SDL_GetTicks();
	}

	[[nodiscard]] int16_t
	framerate() const
	{
		return _framerate;
	}

	[[nodiscard]] int16_t
	framerate_timeout() const
	{
		return static_cast<int16_t>((1.0f / _framerate) * 1000.0f);
	}

private:
	util::WeakReference<State> _state;
	int_fast16_t _fps = 0;
	int64_t _last_tick = 0;
	int64_t _tick = 0;
	int16_t _framerate = 60;
};

} /* namespace euler::game */

#endif /* EULER_GAME_SYSTEM_H */
