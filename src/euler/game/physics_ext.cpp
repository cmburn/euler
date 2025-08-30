/* SPDX-License-Identifier: ISC */

#include "euler/game/physics_ext.h"

void
euler::game::init_physics(util::Reference<State> state)
{
	state->log()->info("Initializing Euler::Physics...");
	auto mrb = state->mrb();
	auto &mod = state->module();
	mod.physics.module = mrb_define_module_under(mrb, mod.module, "Physics");
	state->log()->info("Euler::Physics initialized");
}
