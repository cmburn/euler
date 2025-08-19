/* SPDX-License-Identifier: ISC */

#include <mutex>
#include <stdexcept>

#include "euler/graphics/window.h"

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

static void sdl_init()
{
	static std::once_flag sdl_init_flag;
	std::call_once(sdl_init_flag, []() {
		if (SDL_Init(sdl_init_flags()) != 0)
			throw std::runtime_error(SDL_GetError());
	});
}

euler::graphics::Window::Window(const std::string &title, const int width,
    const int height, const SDL_WindowFlags flags)
{
//	util::sdl_init();
	_window = SDL_CreateWindow(title.c_str(), width, height, flags);
}

euler::graphics::Window::~Window()
{
	if (_window != nullptr) SDL_DestroyWindow(_window);
}


void
euler::graphics::Window::process_event(const SDL_Event &e)
{

}