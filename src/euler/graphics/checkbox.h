/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_CHECKBOX_H
#define EULER_GUI_CHECKBOX_H

#include <string>
#include <string_view>

#include "euler/graphics/element.h"

namespace Euler::Graphics {
class Checkbox : public Element {
public:
	Checkbox(std::string_view label, bool active = false);

	void
	set_label(const std::string_view label)
	{
		_label = label;
	}

	[[nodiscard]] const std::string &
	label() const
	{
		return _label;
	}

	void
	set_active(const bool active)
	{
		_is_active = active;
	}

	[[nodiscard]] bool
	is_active() const
	{
		return _is_active;
	}

protected:
	std::string _label;
	bool _is_active = true;
};
} /* namespace Euler::Graphics */

#endif /* EULER_GUI_CHECKBOX_H */
