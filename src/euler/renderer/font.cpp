/* SPDX-License-Identifier: ISC */

#include "euler/renderer/font.h"

#include "VK2D/Gui.h"

void
Euler::Renderer::Font::populate_info(VK2DGuiFont *font,
    struct nk_font_config *config)
{
	font->height = _height;

}