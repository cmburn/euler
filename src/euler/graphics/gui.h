/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_GUI_H
#define EULER_RENDERER_GUI_H

#include <functional>

#include "SDL3/SDL_events.h"
#include "euler/graphics/font.h"
#include "euler/graphics/widget.h"
#include "euler/util/logger.h"
#include "euler/util/object.h"

struct nk_context;

namespace Euler::Graphics {
class Window;

class Gui : public Util::Object {
public:
	Gui(const Util::Reference<Util::Logger> &logger,
	    const Util::Reference<Window> &window,
	    std::vector<Font> &&fonts = {});
	void process_event(SDL_Event *e);
	void draw(const std::function<void()> &);
	void input(const std::function<void()> &);
	void add_widget(const Util::Reference<Widget> &widget);
	nk_context *
	context() const
	{
		return _ctx;
	}

private:
	void start_input();
	void end_input();
	static void load_fonts(std::vector<Font> &&fonts);
	bool _in_draw = false;
	Util::Reference<Util::Logger> _log;
	Util::Reference<Window> _window;
	std::unordered_map<std::string_view, Util::Reference<Widget>> _widgets;
	nk_context *_ctx = nullptr;
	uint32_t _nk_flags = 0;
	bool _initialized = false;
};
} /* namespace Euler::Graphics */

#endif /* EULER_RENDERER_GUI_H */
