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
	~Window() override;
	void widget(const char *title,
	    const std::function<void(const util::Reference<Widget> &)> &fn,
	    const Widget::Rectangle &rect = {},
	    const Widget::Flags &flags = {});
	void button(const char *title,
	    const std::function<void(const util::Reference<Button> &)> &fn);

	const nk_context *context() const;
	nk_context *context();
	vk::raii::Semaphore gui_render() const override;
	void initialize(const util::Reference<vulkan::Renderer> &renderer);

private:
	SDL_Window *_window = nullptr;
	nk_sdl *_sdl = nullptr;
};

} /* namespace euler::gui */

#endif /* EULER_GUI_GUI_H */
