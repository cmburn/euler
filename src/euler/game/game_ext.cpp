/* SPDX-License-Identifier: ISC */

#include "euler/game/game_ext.h"
#include "../../../extern/mruby/include/mruby/class.h"
#include "euler/game/util_ext.h"

using namespace euler::game;
using Modules = State::Modules;

extern const mrb_data_type euler::game::STATE_TYPE
    = MAKE_REFERENCE_TYPE(euler::game::State);

static constexpr auto state_log = ATTR_READER(State, STATE_TYPE,
    LOGGER_TYPE, mod.util.logger.klass, self->log());
static constexpr auto state_user_storage = ATTR_READER(State, STATE_TYPE,
    STORAGE_TYPE, mod.util.storage, self->user_storage());
static constexpr auto state_title_storage = ATTR_READER(State, STATE_TYPE,
    STORAGE_TYPE, mod.util.storage, self->title_storage());

static mrb_value
state_allocate(mrb_state *mrb, mrb_value self)
{
	auto state = State::get(mrb);
	assert(state != nullptr);
	const auto c = mrb_class_ptr(self);
	const auto obj = Data_Wrap_Struct(mrb, c, &STATE_TYPE, state.wrap());
	return mrb_obj_value(obj);
}

static void
init_state(mrb_state *mrb, Modules &mod)
{
	mod.game.state = mrb_define_class_under(mrb, mod.game.module, "State",
	    mrb->object_class);
	const auto state = mod.game.state;
	mrb_define_method(mrb, state, "log", state_log, MRB_ARGS_NONE());
	mrb_define_method(mrb, state, "user_storage", state_user_storage,
	    MRB_ARGS_NONE());
	mrb_define_method(mrb, state, "title_storage", state_title_storage,
	    MRB_ARGS_NONE());
	mrb_define_class_method(mrb, state, "allocate", state_allocate,
	    MRB_ARGS_NONE());
}

void
euler::game::init_game(util::Reference<State> state)
{
	state->log()->info("Initializing Euler::Game...");
	const auto mrb = state->mrb();
	auto &mod = state->module();
	mod.game.module = mrb_define_module_under(mrb, mod.module, "Game");
	init_state(mrb, mod);
	state->log()->info("Euler::Util initialized");
}