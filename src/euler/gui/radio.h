/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_GUI_RADIO_H
#define EULER_GUI_RADIO_H

namespace euler::gui {
class Radio : public Element {
public:
	Radio(std::string_view label, Alignment widget_alignment,
	    Alignment text_alignment);
};
} /* namespace euler::gui */


#endif /* EULER_GUI_RADIO_H */

