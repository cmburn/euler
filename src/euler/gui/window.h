/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_WINDOW_H
#define EULER_GUI_WINDOW_H

#include <functional>

#include "euler/gui/row.h"
#include "euler/util/object.h"

namespace euler::gui {
class Gui;

class Window final : public util::Object {
public:
	struct Flags {
		bool border : 1 = false;
		bool moveable : 1 = false;
		bool scalable : 1 = false;
		bool closeable : 1 = false;
		bool minimizable : 1 = false;
		bool no_scrollbar : 1 = false;
		bool title : 1 = false;
		bool scroll_auto_hide : 1 = false;
		bool background : 1 = false;
		bool scale_left : 1 = false;
		bool no_input : 1 = false;
	};

	struct Rectangle {
		float x = 0;
		float y = 0;
		float w = 100;
		float h = 100;
	};

	Window(const char *title, const Rectangle &rect, const Flags &flags,
	    const util::Reference<Gui> &gui);

	void row(bool dynamic,
	    std::function<void(const util::Reference<Row> &)> &fn,
	    float height = 24.0f, int cols = 1);

	void call(
	    const std::function<void(const util::Reference<Window> &)> &fn);
	util::Reference<Gui> gui() const;
	~Window() override;

private:
	util::WeakReference<Gui> _gui;
	Rectangle _rect;
	// float x, y, w, h;
	Flags _flags;
};

} /* namespace euler::gui */

#endif /* EULER_GUI_WINDOW_H */
