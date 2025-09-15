/* SPDX-License-Identifier: ISC */

#include "euler/gui/widget.h"
#include "euler/gui/nuklear_defs.h"
#include "euler/gui/window.h"

static nk_uint
to_nk(const euler::gui::Widget::Flags &flags)
{
	nk_uint out = 0;
	if (flags.border) out |= NK_WINDOW_BORDER;
	if (flags.moveable) out |= NK_WINDOW_MOVABLE;
	if (flags.scalable) out |= NK_WINDOW_SCALABLE;
	if (flags.closeable) out |= NK_WINDOW_CLOSABLE;
	if (flags.minimizable) out |= NK_WINDOW_MINIMIZABLE;
	if (flags.no_scrollbar) out |= NK_WINDOW_NO_SCROLLBAR;
	if (flags.title) out |= NK_WINDOW_TITLE;
	if (flags.scroll_auto_hide) out |= NK_WINDOW_SCROLL_AUTO_HIDE;
	if (flags.background) out |= NK_WINDOW_BACKGROUND;
	if (flags.scale_left) out |= NK_WINDOW_SCALE_LEFT;
	if (flags.no_input) out |= NK_WINDOW_NO_INPUT;
	return out;
}

euler::gui::Widget::Widget(const char *title, const Rectangle &rect,
    const Flags &flags, const util::Reference<Window> &gui)
    : _gui(gui)
    , _title(title)
    , _rect(rect)
    , _flags(flags)
{
}

void
euler::gui::Widget::row(bool dynamic,
    std::function<void(const util::Reference<Row> &)> &fn, float height,
    int cols)
{
	const auto row = util::make_reference<Row>(dynamic, height, cols,
	    util::Reference(this));
	row->call(fn);
}

void
euler::gui::Widget::call(
    const std::function<void(const util::Reference<Widget> &)> &fn)
{
	const struct nk_rect rect = {
		.x = _rect.x,
		.y = _rect.y,
		.w = _rect.w,
		.h = _rect.h,
	};

	if (nk_begin(gui()->context(), "Window", rect, to_nk(_flags))) {
		const auto self = util::Reference(this);
		try {
			fn(self);
		} catch (...) {
			nk_end(gui()->context());
			throw;
		}
	}
	nk_end(gui()->context());
}

euler::util::Reference<euler::gui::Window>
euler::gui::Widget::gui() const
{
	return _gui.strengthen();
}