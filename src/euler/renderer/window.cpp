/* SPDX-License-Identifier: ISC */

#include <mutex>

#include "euler/renderer/window.h"

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

Euler::Renderer::Window::Window(const std::string &title, const int width,
    const int height, const SDL_WindowFlags flags)
{
	_window = SDL_CreateWindow(title.c_str(), width, height, flags);
}

Euler::Renderer::Window::~Window()
{
	if (_window != nullptr) SDL_DestroyWindow(_window);
}

void
Euler::Renderer::Window::global_init()
{
	static constexpr int SDL_INIT_FLAGS = sdl_init_flags();
	static std::once_flag once;
	std::call_once(once, []() {
		SDL_Init(sdl_init_flags());
	});
}

Eigen::Vector4f
Euler::Renderer::Window::size() const
{
	Eigen::Vector4f size;
	int w, h, x, y;
	SDL_GetWindowPosition(_window, &x, &y);
	SDL_GetWindowSize(_window, &w, &h);
	return {
		static_cast<float>(x),
		static_cast<float>(y),
		static_cast<float>(w),
		static_cast<float>(h),
	};
}