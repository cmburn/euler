/* SPDX-License-Identifier: ISC */

#include "euler/game/vulkan_ext.h"

void
euler::game::init_vulkan(util::Reference<State> state)
{
	state->log()->info("Initializing Euler::Vulkan...");
	auto mrb = state->mrb();
	auto &mod = state->module();
	mod.util.module = mrb_define_module_under(mrb, mod.module, "Vulkan");
	state->log()->info("Euler::Vulkan initialized");
}