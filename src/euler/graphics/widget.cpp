/* SPDX-License-Identifier: ISC */

#include "euler/graphics/widget.h"

Euler::Graphics::Widget::Config::Config()
{
	title = std::nullopt;
	draw_border = false;
	is_movable = false;
	is_scalable = false;
	is_closable = false;
	is_minimizable = false;
	has_scrollbar = true;
	auto_hide_scrollbar = false;
	keep_in_background = false;
	scaler_on_left = false;
	no_input = false;
}
