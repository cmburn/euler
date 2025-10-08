/* SPDX-License-Identifier: ISC */

#include "euler/gui/window.h"

#include <vulkan/vulkan_raii.hpp>

#include "euler/gui/nuklear_defs.h"
#include "euler/gui/widget.h"

euler::gui::Window::Window(const util::Reference<util::Logger> &parent,
    const std::string &progname) : graphics::Window(parent, progname)
{
}