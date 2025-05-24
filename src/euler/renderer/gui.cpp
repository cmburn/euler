/* SPDX-License-Identifier: ISC */

#include "euler/renderer/gui.h"

#include "VK2D/Gui.h"

#include "euler/renderer/window.h"
#include "euler/util/defer.h"

using FontInfo = std::pair<VK2DGuiFont, std::optional<struct nk_font_config>>;

void
Euler::Renderer::Gui::load_fonts(std::vector<Euler::Renderer::Font> &&fonts)
{
	if (fonts.empty()) return;
	std::vector<VK2DGuiFont> vk2d_fonts;
	// need to retain configs for the lifetime of font init
	std::vector<struct nk_font_config> configs;
	vk2d_fonts.reserve(fonts.size());
	for (auto &font : fonts) {
		VK2DGuiFont gui_font = {};
		auto config = nk_font_config(font._height);
		font.populate_info(&gui_font, &config);
		if (gui_font.config != nullptr) {
			configs.emplace_back(config);
			gui_font.config = &configs.back();
		}
		vk2d_fonts.emplace_back(gui_font);
	}
	vk2dGuiLoadFonts(vk2d_fonts.data(), vk2d_fonts.size());
}

Euler::Renderer::Gui::Gui(const Util::Reference<Util::Logger> &logger,
    const Util::Reference<Window> &window, std::vector<Font> &&fonts)
    : _log(logger)
    , _window(window)
{
	load_fonts(std::move(fonts));
	_ctx = vk2dGuiContext();
}

void
Euler::Renderer::Gui::process_event(SDL_Event *e)
{
	vk2dGuiProcessEvent(e);
}

void
Euler::Renderer::Gui::draw(const std::function<void()> &fn)
{
	const auto vec = _window->size();
	const struct nk_rect bounds = { vec[0], vec[1], vec[2], vec[3] };
	DEFER([&] { nk_end(_ctx); });
	if (nk_begin(_ctx, _window->title().c_str(), bounds, _nk_flags)) fn();
	else _log->warn("Failed to begin GUI render");
}

void
Euler::Renderer::Gui::input(const std::function<void()> &fn)
{
	vk2dGuiStartInput();
	DEFER([] { vk2dGuiEndInput(); });
	fn();
}

void
Euler::Renderer::Gui::start_input()
{
	vk2dGuiStartInput();
}

void
Euler::Renderer::Gui::end_input()
{
	vk2dGuiEndInput();
}