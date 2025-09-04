/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_BUTTON_H
#define EULER_GUI_BUTTON_H

#include "euler/gui/element.h"
#include "euler/gui/common.h"
#include "euler/graphics/color.h"

namespace euler::gui {

class Button final : public Element {
public:
	~Button() override = default;
	Button(std::string_view label, graphics::Color color,
	    Symbol symbol);

	graphics::Color color() const { return _color; }
	const std::string &label() const { return _label; }
	Symbol symbol() const { return _symbol; }

private:
	std::string _label;
	graphics::Color _color;
	Symbol _symbol;
};
} /* namespace euler::gui */


#endif /* EULER_GUI_BUTTON_H */

