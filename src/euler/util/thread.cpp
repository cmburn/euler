/* SPDX-License-Identifier: ISC */

#include "euler/util/thread.h"

#include <thread>

#include "SDL3/SDL_init.h"

bool
Euler::Util::on_main_thread()
{
	return SDL_IsMainThread();
}