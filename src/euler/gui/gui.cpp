/* SPDX-License-Identifier: ISC */

#include "euler/gui/gui.h"
#include "euler/gui/window.h"
#include "euler/gui/nuklear_shim.h"

void
euler::gui::Gui::window(const char *title,
    const std::function<void(const util::Reference<Window> &)> &fn,
    const Window::Rectangle &rect, const Window::Flags &flags)
{
	auto win = util::make_reference<Window>(title, rect, flags, util::Reference(this));
	win->call(fn);
}

void
euler::gui::Gui::button(const char *title,
    const std::function<void(const util::Reference<Button> &)> &fn)
{

}