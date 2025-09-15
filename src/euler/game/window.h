/* SPDX-License-Identifier: ISC */

#ifndef EULER_GAME_WINDOW_H
#define EULER_GAME_WINDOW_H

#include "euler/gui/window.h"
#include "euler/util/object.h"

namespace euler::game {
class Window final : public gui::Window {
public:
	Window(const util::Reference<util::Logger> &log, const std::string &progname);
	~Window() override = default;
};
} /* namespace euler::game */


#endif /* EULER_GAME_WINDOW_H */

