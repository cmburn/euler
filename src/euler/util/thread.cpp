/* SPDX-License-Identifier: ISC */

#include <cassert>

#include "euler/util/thread.h"

#include <thread>

#include "SDL3/SDL_init.h"
#include "euler/util/sdl.h"

bool
Euler::Util::is_main_thread()
{
	return SDL_IsMainThread();
}

bool
Euler::Util::run_on_main_thread(const Callback &fn, const bool wait_complete)
{
	assert(is_sdl_init());
	if (is_main_thread()) {
		fn();
		return true;
	}
	static constexpr SDL_MainThreadCallback CALLBACK = [](void *data) {
		const auto ptr = static_cast<std::function<void()> *>(data);
		(*ptr)();
	};
	auto *ptr = const_cast<Callback *>(&fn);
	return SDL_RunOnMainThread(CALLBACK, ptr, wait_complete);
}