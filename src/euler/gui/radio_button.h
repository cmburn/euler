/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_RADIO_BUTTON_H
#define EULER_GUI_RADIO_BUTTON_H

#include <string_view>

#include "euler/gui/element.h"

namespace euler::gui {
class RadioButton : public Element {
public:
	RadioButton(std::string_view label, Alignment widget_alignment,
	    Alignment text_alignment);
};
} /* namespace euler::gui */

#endif /* EULER_GUI_RADIO_H */
