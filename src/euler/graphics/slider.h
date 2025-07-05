/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_SLIDER_H
#define EULER_GUI_SLIDER_H

#include "euler/graphics/element.h"

namespace Euler::Graphics {
class Slider : public Element {
public:
	Slider(float min, float max, int step);
	float value() const;

};
} /* namespace Euler::Graphics */

#endif /* EULER_GUI_SLIDER_H */

