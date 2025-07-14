/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_GUI_SLIDER_H
#define EULER_GUI_SLIDER_H

namespace euler::gui {
class Slider : public Element {
public:
	Slider(float min, float max, int step);
	float value() const;

};
} /* namespace euler::gui */


#endif /* EULER_GUI_SLIDER_H */

