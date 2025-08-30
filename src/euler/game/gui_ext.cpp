/* SPDX-License-Identifier: ISC */

#include "euler/game/gui_ext.h"

void
euler::game::init_gui(util::Reference<State> state)
{
	state->log()->info("Initializing Euler::GUI...");
	auto mrb = state->mrb();
	auto &mod = state->module();
	mod.gui.module = mrb_define_module_under(mrb, mod.module, "GUI");
	state->log()->info("Euler::GUI initialized");
}