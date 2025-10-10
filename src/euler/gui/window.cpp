/* SPDX-License-Identifier: ISC */

#include "euler/gui/window.h"

#include "euler/gui/nuklear_defs.h"
#include "euler/gui/widget.h"

#include <VK2D/Gui.h>

bool
euler::gui::Window::process_gui_event(SDL_Event &e)
{
	vk2dGuiProcessEvent(&e);
	return true;
}

euler::gui::Window::Window(const util::Reference<util::Logger> &parent,
    const std::string &progname) : graphics::Window(parent, progname)
{
}
