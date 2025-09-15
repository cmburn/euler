/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_WIDGET_H
#define EULER_GUI_WIDGET_H

#include <functional>
#include <string>

#include "euler/gui/row.h"
#include "euler/util/object.h"

namespace euler::gui {
class Window;

class Widget final : public util::Object {
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

	Widget(const char *title, const Rectangle &rect, const Flags &flags,
	    const util::Reference<Window> &gui);

	void row(bool dynamic,
	    std::function<void(const util::Reference<Row> &)> &fn,
	    float height = 24.0f, int cols = 1);

	void call(
	    const std::function<void(const util::Reference<Widget> &)> &fn);
	util::Reference<Window> gui() const;
	~Widget() override = default;

private:
	util::WeakReference<Window> _gui;
	std::string _title;
	Rectangle _rect;
	Flags _flags;
};

} /* namespace euler::gui */

#endif /* EULER_GUI_WIDGET_H */
