/* SPDX-License-Identifier: ISC */

#include "euler/app/game_ext.h"

#include <mruby/class.h>
#include <mruby/variable.h>

#include "euler/app/util_ext.h"
#include "euler/app/event.h"

using namespace euler::app;
using Modules = State::Modules;

extern const mrb_data_type euler::app::STATE_TYPE
	= MAKE_REFERENCE_TYPE(euler::app::State);
extern const mrb_data_type euler::app::SYSTEM_TYPE
	= MAKE_REFERENCE_TYPE(euler::app::System);

static constexpr auto state_log = ATTR_READER(State, STATE_TYPE, LOGGER_TYPE,
		mod.util.logger.klass, self->log());
static constexpr auto state_user_storage = ATTR_READER(State, STATE_TYPE,
		STORAGE_TYPE, mod.util.storage, self->user_storage());
static constexpr auto state_title_storage = ATTR_READER(State, STATE_TYPE,
		STORAGE_TYPE, mod.util.storage, self->title_storage());
// static constexpr auto state_system = ATTR_IV_READER(system);
#define WRAP(ARG) mrb_float_value(mrb, ARG)
// static cosntexpr auto system_fps = ATTR_READER(System, SYSTEM_TYPE,
// 	mrb_float, mrb->float_class, self->fps());

static constexpr auto system_fps
	= [](mrb_state *mrb, const mrb_value self_value) {
	const auto state = State::get(mrb);
	const auto self = unwrap_data<System>(mrb, self_value, &SYSTEM_TYPE);
	state->assert_state();
	auto ans = (self->fps());
	state->assert_state();
	return WRAP(ans);
};
#undef WRAP

static mrb_value
state_system(mrb_state *mrb, mrb_value self)
{
	return mrb_iv_get(mrb, self, MRB_IVSYM(system));
}

static mrb_value
state_allocate(mrb_state *mrb, mrb_value self)
{
	auto state = State::get(mrb);
	assert(state != nullptr);
	const auto c = mrb_class_ptr(self);
	const auto obj = Data_Wrap_Struct(mrb, c, &STATE_TYPE, state.wrap());
	return mrb_obj_value(obj);
}

// static mrb_value
// system_fps(mrb_state *state, const mrb_value self)
// {
// 	const auto system = euler::util::Reference<System>::unwrap(self);
// 	(void)state;
// 	assert(system != nullptr);
// 	return mrb_fixnum_value(system->fps());
// }

static void
init_system(mrb_state *mrb, Modules &mod)
{
	mod.app.system = mrb_define_class_under(mrb, mod.app.module, "System",
		mrb->object_class);
	const auto system = mod.app.system;
	mrb_define_method(mrb, system, "fps", system_fps, MRB_ARGS_NONE());
	MRB_SET_INSTANCE_TT(system, MRB_TT_CDATA);
}

static void
init_state(mrb_state *mrb, Modules &mod)
{
	mod.app.state = mrb_define_class_under(mrb, mod.app.module, "State",
		mrb->object_class);
	const auto state = mod.app.state;
	MRB_SET_INSTANCE_TT(state, MRB_TT_CDATA);
	mrb_define_method(mrb, state, "log", state_log, MRB_ARGS_NONE());
	mrb_define_method(mrb, state, "user_storage", state_user_storage,
		MRB_ARGS_NONE());
	mrb_define_method(mrb, state, "title_storage", state_title_storage,
		MRB_ARGS_NONE());
	mrb_define_method(mrb, state, "system", state_system, MRB_ARGS_NONE());

	mrb_define_class_method(mrb, state, "allocate", state_allocate,
		MRB_ARGS_NONE());
}

void
euler::app::init_app(util::Reference<State> state)
{
	state->log()->info("Initializing Euler::App...");
	const auto mrb = state->mrb();
	auto &mod = state->module();
	mod.app.module = mrb_define_module_under(mrb, mod.module, "App");
	init_state(mrb, mod);
	init_system(mrb, mod);
	init_app_event(state);
	state->log()->info("Euler::Util initialized");
}