/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_ELEMENT_H
#define EULER_GUI_ELEMENT_H
#include "euler/util/object.h"
#include "euler/graphics/window.h"

namespace Euler::Graphics {
class Element : public Util::Object {
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
	virtual void render(const std::shared_ptr<Window> &) = 0;
};
} /* namespace Euler::Graphics */

#endif /* EULER_GUI_ELEMENT_H */

