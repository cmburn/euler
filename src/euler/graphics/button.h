/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_BUTTON_H
#define EULER_GUI_BUTTON_H

#include "euler/graphics/color.h"
#include "euler/graphics/element.h"
#include "euler/graphics/symbol.h"

namespace Euler::Graphics {

class Button : public Element {
public:
	~Button() override = default;
	Button(std::string_view label, Color color,
	    Symbol symbol);

private:
	std::string _label;
	Color _color;
	Symbol _symbol;
};

} /* namespace Euler::Graphics */

#endif /* EULER_GUI_BUTTON_H */
