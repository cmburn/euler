/* SPDX-License-Identifier: ISC */

#include <mutex>

#include "euler/util/sdl.h"

#include "SDL3/SDL.h"

static constexpr int
sdl_init_flags()
{
	int flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS;
#ifdef EULER_USE_JOYSTICK
	flags |= SDL_INIT_JOYSTICK;
#endif
#ifdef EULER_USE_HAPTIC
	flags |= SDL_INIT_HAPTIC;
#endif
#ifdef EULER_USE_GAMEPAD
	flags |= SDL_INIT_GAMEPAD;
#endif
#ifdef EULER_USE_SENSOR
	flags |= SDL_INIT_SENSOR;
#endif
#ifdef EULER_USE_CAMERA
	flags |= SDL_INIT_CAMERA;
#endif
	return flags;
}


void
euler::util::sdl_init()
{
	static std::once_flag once;
	std::call_once(once, []() {
		static constexpr int SDL_INIT_FLAGS = sdl_init_flags();
		SDL_Init(SDL_INIT_FLAGS);
	});
}

bool
euler::util::is_sdl_init()
{
	return SDL_WasInit(sdl_init_flags()) != 0;
}


