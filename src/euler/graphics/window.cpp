/* SPDX-License-Identifier: ISC */

#include <mutex>
#include <stdexcept>

#include "euler/graphics/window.h"



euler::graphics::Window::Window(const std::string &title, const int width,
    const int height, const SDL_WindowFlags flags)
{
	_window = SDL_CreateWindow(title.c_str(), width, height, flags);
}

euler::graphics::Window::~Window()
{
	if (_window != nullptr) SDL_DestroyWindow(_window);
}


void
euler::graphics::Window::process_event(const SDL_Event &)
{

}