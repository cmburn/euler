/* SPDX-License-Identifier: ISC */

#include "euler/game/graphics_ext.h"

void
euler::game::init_graphics(util::Reference<State> state)
{
	state->log()->info("Initializing Euler::Graphics...");
	auto mrb = state->mrb();
	auto &mod = state->module();
	mod.graphics.module = mrb_define_module_under(mrb, mod.module, "Graphics");
	state->log()->info("Euler::Graphics initialized");
}