/* SPDX-License-Identifier: ISC */

#include <mutex>
#include <stdexcept>

#include "euler/graphics/window.h"

euler::graphics::Window::Window(const util::Reference<util::Logger> &log,
    const std::string &title, const int width, const int height,
    const SDL_WindowFlags flags)
    : _title(title)
    , _log(log->copy("graphics"))
{
	_window = SDL_CreateWindow(title.c_str(), width, height, flags);
	_log->info("Window created");
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

bool
euler::graphics::Window::poll_event(SDL_Event &e,
    const std::function<bool(SDL_Event &)> &fn)
{
	bool quit = false;
	start_input();
	[[maybe_unused]] auto guard = input_guard();
	while (SDL_PollEvent(&e)) {
		_log->debug("Received event {}", e.type);
		quit = !fn(e);
		if (quit) break;
		if (process_gui_event(e)) continue;
		quit = true;
		break;
	}
	end_input();
	return !quit;
}

void
euler::graphics::Window::start_input()
{
	start_gui_input();
}

void
euler::graphics::Window::end_input()
{
	end_gui_input();
}
