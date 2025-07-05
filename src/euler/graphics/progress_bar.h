/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_PROGRESS_BAR_H
#define EULER_GUI_PROGRESS_BAR_H

#include "euler/graphics/element.h"

namespace Euler::Graphics {
class ProgressBar : public Element {
public:
	ProgressBar(size_t max, bool modifiable);

private:
	size_t _max;
	bool _modifiable;
};
} /* namespace Euler::Graphics */

#endif /* EULER_GUI_PROGRESS_BAR_H */

