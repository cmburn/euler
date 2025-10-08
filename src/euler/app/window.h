/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_WINDOW_H
#define EULER_APP_WINDOW_H

#include "euler/gui/window.h"
#include "euler/util/object.h"

namespace euler::app {
class Window final : public gui::Window {
public:
	Window(const util::Reference<util::Logger> &log, const std::string &progname);
	~Window() override = default;

};
} /* namespace euler::app */


#endif /* EULER_APP_WINDOW_H */

