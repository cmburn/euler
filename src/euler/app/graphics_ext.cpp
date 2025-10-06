/* SPDX-License-Identifier: ISC */

#include "euler/app/graphics_ext.h"

void
euler::app::init_graphics(util::Reference<State> state)
{
	state->log()->info("Initializing Euler::Graphics...");
	auto mrb = state->mrb();
	auto &mod = state->module();
	mod.graphics.module = mrb_define_module_under(mrb, mod.module, "Graphics");
	state->log()->info("Euler::Graphics initialized");
}