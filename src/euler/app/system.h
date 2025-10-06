/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_SYSTEM_H
#define EULER_APP_SYSTEM_H

#include <SDL3/SDL_timer.h>

#include "euler/util/object.h"

namespace euler::app {
class State;

/*
 * Primarily responsible for miscellaneous SDL and timing functions.
 * Only separated from State for clarity.
 */
class System final : public util::Object {
	friend class State;

public:
	System(const util::Reference<State> &state = nullptr);
	~System() override;
	util::Reference<State> state() const;
	using tick_t = decltype(SDL_GetTicks());

	[[nodiscard]] float
	dt() const
	{
		return (_tick - _last_tick) / 1000.0f;
	}

	[[nodiscard]] float
	fps() const
	{
		return _fps;
	}

	void tick();

	[[nodiscard]] int64_t
	ticks() const
	{
		return _tick;
	}

	[[nodiscard]] int64_t
	total() const
	{
		return _total;
	}

	// [[nodiscard]] int16_t
	// framerate() const
	// {
	// 	return _framerate;
	// }
	//
	// [[nodiscard]] int16_t
	// framerate_timeout() const
	// {
	// 	return static_cast<int16_t>((1.0f / _framerate) * 1000.0f);
	// }


private:
	util::WeakReference<State> _state;
	float _fps = 0;
	tick_t _last_tick = 0;
	tick_t _tick = 0;
	tick_t _total = 0;
	// frames elapsed over the last second
	tick_t _frames = 0;
	tick_t _last_frames_tick = 0;
	tick_t _last_frames_total = 0;
};

} /* namespace euler::app */

#endif /* EULER_APP_SYSTEM_H */
