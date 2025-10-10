/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_GUI_H
#define EULER_GUI_GUI_H

#include <functional>

#include "euler/graphics/window.h"
#include "euler/gui/button.h"
#include "euler/gui/widget.h"
#include "euler/util/object.h"

struct nk_context;
struct nk_sdl;

namespace euler::gui {

/*
 * Our GUI library is heavily patterned after
 * https://github.com/keharriso/love-nuklear/ but adapted to mesh better with
 * our engine architecture and mRuby. Both use Nuklear as the underlying GUI
 * library.
 */

class Window : public graphics::Window {
public:
	Window(const util::Reference<util::Logger> &parent,
	    const std::string &progname);
	~Window() override = default;
	void widget(const char *title,
	    const std::function<void(const util::Reference<Widget> &)> &fn,
	    const Widget::Rectangle &rect = {},
	    const Widget::Flags &flags = {});
	void button(const char *title,
	    const std::function<void(const util::Reference<Button> &)> &fn);

	bool process_gui_event(SDL_Event &e) override;

	const nk_context *
	context() const
	{
		return renderer()->gui_context();
	}

	nk_context *
	context()
	{
		return renderer()->gui_context();
	}
};

} /* namespace euler::gui */

#endif /* EULER_GUI_GUI_H */
