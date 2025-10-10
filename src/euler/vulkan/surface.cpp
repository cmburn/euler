/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/surface.h"
#include "VK2D/Gui.h"
#include "VK2D/Renderer.h"

#include "euler/vulkan/renderer.h"

euler::vulkan::Surface::Surface() = default;
euler::vulkan::Surface::~Surface() = default;

const euler::util::Reference<euler::vulkan::Renderer> &
euler::vulkan::Surface::renderer() const
{
	return _renderer;
}

euler::util::Reference<euler::vulkan::Renderer> &
euler::vulkan::Surface::renderer()
{
	return _renderer;
}
bool
euler::vulkan::Surface::draw(int &exit_code,
    const std::function<bool(int &)> &fn)
{
	vk2dRendererStartFrame(util::BLACK.to_float_array().data());
	try {
		const auto result = fn(exit_code);
		vk2dRendererEndFrame();
		return result;
	} catch (const std::exception &e) {
		log()->error("Unhandled exception in frame: {}", e.what());
	} catch (...) {
		log()->error("Unknown exception in frame");
	}
	vk2dRendererEndFrame();
	return false;
}

void
euler::vulkan::Surface::test_gui()
{
	float value = 0.0f;
	int op = 0;

	nk_clear(vk2dGuiContext());
	if (nk_begin(vk2dGuiContext(), "Show", nk_rect(50, 50, 220, 220),
		NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE)) {
		nk_layout_row_static(vk2dGuiContext(), 30, 80, 1);
		if (nk_button_label(vk2dGuiContext(), "button")) {
			log()->info("Button pressed!");
		}
	}
	nk_end(vk2dGuiContext());
}

/* ReSharper disable once CppMemberFunctionMayBeStatic */
void
euler::vulkan::Surface::start_gui_input()
{
	vk2dGuiStartInput();
}

/* ReSharper disable once CppMemberFunctionMayBeStatic */
void
euler::vulkan::Surface::end_gui_input()
{
	vk2dGuiEndInput();
	nk_clear(vk2dGuiContext());
}

void
euler::vulkan::Surface::set_renderer(const util::Reference<Renderer> &renderer)
{
	_renderer = renderer;
}