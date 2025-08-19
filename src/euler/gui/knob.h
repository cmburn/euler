/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_KNOB_H
#define EULER_GUI_KNOB_H

#include "euler/util/object.h"

#include "euler/gui/element.h"

namespace euler::gui {
class Knob : public Element {
public:
	enum class Heading {
		Up,
		Right,
		Down,
		Left,
	};

	Knob(float min, float max, float step,
	    Heading zero_direction = Heading::Up,
	    float dead_zone_degrees = 0.0f);

	float value() const;
private:
	float _min;
	float _max;
	float _step;
	Heading _zero_direction;
	float _dead_zone_degrees;
};
} /* namespace euler::gui */


#endif /* EULER_GUI_KNOB_H */

