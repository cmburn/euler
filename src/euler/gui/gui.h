/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_GUI_H
#define EULER_GUI_GUI_H

#include <functional>

#include "euler/gui/button.h"
#include "euler/gui/window.h"
#include "euler/util/object.h"

struct nk_context;

namespace euler::gui {

/*
 * Our GUI library is heavily patterned after
 * https://github.com/keharriso/love-nuklear/ but adapted to mesh better with
 * our engine architecture and mRuby. Both use Nuklear as the underlying GUI
 * library.
 */

/**
 * An Gui represents one GUI state. There should only be one Gui per
 * window.
 */
class Gui final : public util::Object {
public:
	Gui(const util::WeakReference<util::Logger> &parent);
	~Gui() override;
	void window(const char *title,
	    const std::function<void(const util::Reference<Window> &)> &fn,
	    const Window::Rectangle &rect = {},
	    const Window::Flags &flags = {});
	void button(const char *title,
	    const std::function<void(const util::Reference<Button> &)> &fn);

	const nk_context *
	context() const
	{
		return _ctx;
	}

	nk_context *
	context()
	{
		return _ctx;
	}

private:
	nk_context *_ctx = nullptr;
};

} /* namespace euler::gui */

#endif /* EULER_GUI_GUI_H */
