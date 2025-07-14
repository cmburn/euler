/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_GUI_GUI_H
#define EULER_GUI_GUI_H

namespace euler::gui {
class Window;

class Gui : public util::Object {
public:
	Gui(const util::Reference<util::Logger> &logger,
	    const util::Reference<Window> &window,
	    std::vector<Font> &&fonts = {});
	void process_event(SDL_Event *e);
	void draw(const std::function<void()> &);
	void input(const std::function<void()> &);
	void add_widget(const util::Reference<Widget> &widget);

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
	util::Reference<util::Logger> _log;
	util::Reference<Window> _window;
	std::unordered_map<std::string_view, util::Reference<Widget>> _widgets;
	nk_context *_ctx = nullptr;
	uint32_t _nk_flags = 0;
	bool _initialized = false;
};

} /* namespace euler::gui */


#endif /* EULER_GUI_GUI_H */

