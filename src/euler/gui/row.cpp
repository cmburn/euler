/* SPDX-License-Identifier: ISC */

#include "euler/gui/row.h"

#include "euler/gui/widget.h"

euler::gui::Row::Row(const bool dynamic, const float height, const int cols,
    const util::Reference<Widget> &window)
    : _window(window)
    , _dynamic(dynamic)
    , _height(height)
    , _cols(cols)
{
}

void
euler::gui::Row::call(std::function<void(const util::Reference<Row> &)> &fn)
{
	fn(util::Reference(this));
}

euler::util::Reference<euler::gui::Widget>
euler::gui::Row::window() const
{
	return _window.strengthen();
}