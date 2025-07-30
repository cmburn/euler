/* SPDX-License-Identifier: ISC */

#include "euler/util/thread.h"

#include <format>

#include <SDL3/SDL_init.h>

bool
euler::util::is_main_thread()
{
	return SDL_IsMainThread();
}

void
euler::util::run_on_main_thread(const std::function<void()> &fn,
    const bool wait)
{
	using Fn = std::function<void()>;
	const auto ptr = const_cast<Fn *>(&fn);
	const auto result = SDL_RunOnMainThread(
	    [](void *data) { (*static_cast<Fn *>(data))(); }, ptr, wait);
	if (!result) {
		const auto msg = std::format("Failed to run on main thread: {}",
		    SDL_GetError());
		throw std::runtime_error(msg);
	}
}