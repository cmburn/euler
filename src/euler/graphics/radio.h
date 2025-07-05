/* SPDX-License-Identifier: ISC */

#ifndef EULER_GRAPHICS_RADIO_H
#define EULER_GRAPHICS_RADIO_H

#include "euler/graphics/element.h"

namespace Euler::Graphics {
class Radio : public Element {
public:
	Radio(std::string_view label, Alignment widget_alignment,
	    Alignment text_alignment);
};
} /* namespace Euler::Graphics */

#endif /* EULER_GRAPHICS_RADIO_H */
