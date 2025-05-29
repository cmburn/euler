/* SPDX-License-Identifier: ISC */

#include "euler/graphics/font.h"

#include "VK2D/Gui.h"

void
Euler::Graphics::Font::populate_info(VK2DGuiFont *font,
    struct nk_font_config *config)
{
	font->height = _height;

}