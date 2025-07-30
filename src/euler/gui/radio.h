/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_RADIO_H
#define EULER_GUI_RADIO_H

#include <string_view>

#include "euler/gui/element.h"

namespace euler::gui {
class Radio : public Element {
public:
	Radio(std::string_view label, Alignment widget_alignment,
	    Alignment text_alignment);
};
} /* namespace euler::gui */

#endif /* EULER_GUI_RADIO_H */
