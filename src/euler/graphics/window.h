/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_WINDOW_H
#define EULER_RENDERER_WINDOW_H

#include <string>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include "euler/util/object.h"

namespace euler::graphics {
class Window final : public util::Object {
public:
	static constexpr auto DEFAULT_FLAGS = SDL_WINDOW_VULKAN
	    | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
	static constexpr int DEFAULT_WIDTH = 1280;
	static constexpr int DEFAULT_HEIGHT = 720;
	Window(const std::string &title = "Euler Game",
	    int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT,
	    SDL_WindowFlags flags = DEFAULT_FLAGS);
	~Window() override;

	glm::vec4 size() const;
	const std::string &title() const { return _title; }
	SDL_Window *window() { return _window; }
	const SDL_Window *window() const { return _window; }
	void process_event(const SDL_Event &e);

private:
	struct InputGuard {
		util::Reference<Window> window;
		~InputGuard() { window->end_input(); }
	};

public:
	InputGuard input_guard() {
		start_input();
		return InputGuard { util::Reference<Window>(this) };
	}

private:
	friend struct InputGuard;
	void start_input();
	void end_input();
	std::string _title;
	SDL_Window *_window = nullptr;
};
} /* namespace Euler::Graphics */

#endif /* EULER_RENDERER_WINDOW_H */
