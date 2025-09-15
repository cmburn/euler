/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_WINDOW_H
#define EULER_RENDERER_WINDOW_H

#include <string>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include "euler/util/logger.h"
#include "euler/util/object.h"
#include "euler/vulkan/surface.h"

namespace euler::graphics {
class Window : public vulkan::Surface {
	struct InputGuard {
		util::Reference<Window> window;
		~InputGuard() { window->end_input(); }
	};

public:
	static constexpr auto DEFAULT_FLAGS = SDL_WINDOW_VULKAN
	    | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
	static constexpr int DEFAULT_WIDTH = 1280;
	static constexpr int DEFAULT_HEIGHT = 720;
	Window(const util::Reference<util::Logger> &log,
	    const std::string &title = "Euler Game", int width = DEFAULT_WIDTH,
	    int height = DEFAULT_HEIGHT, SDL_WindowFlags flags = DEFAULT_FLAGS);
	~Window() override;

	uint32_t width() const override;
	uint32_t height() const override;

	const std::string &
	title() const
	{
		return _title;
	}

	SDL_Window *
	window() const override
	{
		return _window;
	}

	InputGuard
	input_guard()
	{
		start_input();
		return InputGuard { util::Reference<Window>(this) };
	}

	util::Reference<util::Logger>
	log() const override
	{
		return _log;
	}

private:
	friend struct InputGuard;
	void start_input();
	void end_input();

	std::string _title;
	SDL_Window *_window = nullptr;
	util::Reference<util::Logger> _log;
};
} /* namespace Euler::Graphics */

#endif /* EULER_RENDERER_WINDOW_H */
