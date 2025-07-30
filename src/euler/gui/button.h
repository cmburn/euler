/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_BUTTON_H
#define EULER_GUI_BUTTON_H

#include "euler/util/object.h"

namespace euler::gui {

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
} /* namespace euler::gui */


#endif /* EULER_GUI_BUTTON_H */

