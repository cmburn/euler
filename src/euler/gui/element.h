/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_GUI_ELEMENT_H
#define EULER_GUI_ELEMENT_H

#include "euler/util/object.h"

namespace euler::gui {
class Element : public util::Object {
public:
	enum class Alignment {
		Left = 1,
		HCenter = 2,
		Right = 3,
		Top = 1 << 2,
		VCenter = 2 << 2,
		Bottom = 3 << 2,
	};

	~Element() override = default;
};
} /* namespace euler::gui */

#endif /* EULER_GUI_ELEMENT_H */
