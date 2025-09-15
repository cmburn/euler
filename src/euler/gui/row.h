/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_ROW_H
#define EULER_GUI_ROW_H

#include <functional>

#include "euler/util/object.h"

namespace euler::gui {
class Widget;

class Row final : public util::Object {
public:
	Row(bool dynamic, float height, int cols,
	    const util::Reference<Widget> &window);

	void call(std::function<void(const util::Reference<Row> &)> &fn);
	~Row() override = default;

	util::Reference<Widget> window() const;

private:
	util::WeakReference<Widget> _window;
	bool _dynamic = false;
	float _height = 24.0f;
	int _cols = 1;
	bool _closed = false;
};
} /* namespace euler::gui */


#endif /* EULER_GUI_ROW_H */

