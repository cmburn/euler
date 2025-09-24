/* SPDX-License-Identifier: ISC */

#include "euler/gui/window.h"

#include <vulkan/vulkan_raii.hpp>

#include "euler/gui/nuklear_defs.h"
#include "euler/gui/widget.h"

euler::gui::Window::Window(const util::Reference<util::Logger> &parent,
    const std::string &progname)
    : graphics::Window(parent, progname)
{
}

euler::gui::Window::~Window()
{
	if (_sdl != nullptr) nk_sdl_shutdown(_sdl);
}

void
euler::gui::Window::widget(const char *title,
    const std::function<void(const util::Reference<Widget> &)> &fn,
    const Widget::Rectangle &rect, const Widget::Flags &flags)
{
	const auto win = util::make_reference<Widget>(title, rect, flags,
	    util::Reference(this));
	win->call(fn);
}

void
euler::gui::Window::button(const char *,
    const std::function<void(const util::Reference<Button> &)> &)
{
}

const nk_context *
euler::gui::Window::context() const
{
	return nk_sdl_context(_sdl);
}

nk_context *
euler::gui::Window::context()
{
	return nk_sdl_context(_sdl);
}

vk::raii::Semaphore
euler::gui::Window::gui_render() const
{
	const auto &dev = renderer()->device().device();
	const auto &queue = renderer()->device().queue();
	const auto &rf = render_finished();
	const auto semaphore = nk_sdl_render(_sdl, *queue, image_index(), *rf,
	    NK_ANTI_ALIASING_ON);
	return vk::raii::Semaphore(dev, semaphore);
}