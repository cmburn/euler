/* SPDX-License-Identifier: ISC */

#include <mutex>
#include <stdexcept>

#include "euler/graphics/window.h"

euler::graphics::Window::Window(const util::Reference<util::Logger> &log,
    const std::string &title, const int width, const int height,
    const SDL_WindowFlags flags)
    : _title(title)
    , _log(log)
{
	_window = SDL_CreateWindow(title.c_str(), width, height, flags);
}

euler::graphics::Window::~Window()
{
	if (_window != nullptr) SDL_DestroyWindow(_window);
}

uint32_t
euler::graphics::Window::width() const
{
	return SDL_GetWindowSurface(_window)->w;
}

uint32_t
euler::graphics::Window::height() const
{
	return SDL_GetWindowSurface(_window)->h;
}

