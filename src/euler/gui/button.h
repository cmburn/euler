/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_GUI_BUTTON_H
#define EULER_GUI_BUTTON_H

#include <string>
#include <string_view>

#include "euler/gui/element.h"
#include "euler/gui/symbol.h"
#include "euler/graphics/color.h"

namespace euler::gui {
class Button : public Element {
public:
	~Button() override = default;
	Button(std:: string_view label, graphics::Color color,
	    Symbol symbol);

private:
	std::string _label;
	graphics::Color _color;
	Symbol _symbol;
};

} /* namespace euler::gui */


#endif /* EULER_GUI_BUTTON_H */

