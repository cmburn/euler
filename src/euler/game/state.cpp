/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#include <semaphore>

#include "euler/game/state.h"

#include <mruby/proc.h>
/* mruby/proc.h has to come before mruby/internal.h */
#include <mruby/compile.h>
#include <mruby/internal.h>

#include "euler/game/ext.h"
#include "euler/graphics/ext.h"
#include "euler/gui/ext.h"
#include "euler/physics/ext.h"
#include "euler/util/ext.h"
#include "euler/util/optparse.h"
#include "euler/util/storage.h"

static auto state_count = std::binary_semaphore(0);

euler::game::State::State(const int argc, char **argv)
    : State(Config(argc, argv))
{
}

euler::game::State::State(const Config &config)
{
	_log = make_object<util::Logger>(config.log_level, config.progname);
	_log->info("Initializing state");
	if (!state_count.try_acquire())
		_log->fatal("Multiple states are not yet supported");
	init_fs(config.progname.c_str());
	_state = mrb_open();
	if (_state == nullptr) _log->fatal("Failed to initialize mruby state");
	_log->debug("mrb_state initialized");

	if (config.module_path.empty() || !exists(config.module_path))
		_log->fatal("Module path '{}' is not a valid file");

	/* TODO: proper physfs limiting for module load */
	auto self = util::Reference(this);
	Init_euler_util(self);
	Init_euler_game(self);
	graphics::Init_euler_graphics(self);
	gui::Init_euler_gui(self);
	physics::Init_euler_physics(self);
	_log->debug("Core modules initialized");
}

/* Kernel#require implementation */
bool
euler::game::State::require(const std::string_view path)
{
	if (_loaded_modules.contains(path.data())) {
		_log->debug("Module '{}' already loaded", path);
		return false;
	}
	auto data = _storage->load_text_file(path);
	auto ci = _state->c->ci > _state->c->cibase ? &_state->c->ci[-1]
						    : _state->c->cibase;
	auto scope = ci->proc;
	auto ctx = mrb_ccontext_new(_state);
	ctx->capture_errors = true;
	if (scope != nullptr && MRB_PROC_CFUNC_P(scope)) ctx->upper = nullptr;
	else ctx->upper = scope;
	auto p = mrb_parse_nstring(_state, data.data(), data.size(), ctx);
	if (p == nullptr) {
		log()->error("Failed to parse module '{}', out of memory!",
		    path);
		mrb_ccontext_free(_state, ctx);
		return false;
	}
	if (p->nerr > 0) {
		for (int i = 0; i < p->nerr; i++) {
			const auto &[lineno, column, message]
			    = p->error_buffer[i];
			_log->fatal("Error in module '{}': {} at line {}: {}",
			    path, message, lineno, column);
		}
		mrb_ccontext_free(_state, ctx);
		mrb_parser_free(p);
		return false;
	}
	auto proc = mrb_generate_code(_state, p);
	if (proc == nullptr) {
		_log->fatal("Failed to generate code for module '{}'", path);
		mrb_ccontext_free(_state, ctx);
		mrb_parser_free(p);
		return false;
	}
	auto target_class = MRB_PROC_TARGET_CLASS(scope);
	if (scope != nullptr) {
		REnv *env = mrb_vm_ci_env(ci);
		if (env == nullptr) {
			const auto n_stacks = ci->proc->body.irep->nlocals;
			env = mrb_env_new(_state, _state->c, ci, n_stacks,
			    ci->stack, target_class);
			ci->u.env = env;
		}
		proc->e.env = env;
		proc->flags |= MRB_PROC_ENVSET;
		mrb_field_write_barrier(_state,
		    reinterpret_cast<RBasic *>(proc),
		    reinterpret_cast<RBasic *>(env));
	}
	proc->upper = scope;
	mrb_vm_ci_target_class_set(_state->c->ci, target_class);
	mrb_parser_free(p);
	mrb_ccontext_free(_state, ctx);
	ci->n = 0;
	ci->nk = 0;
	MRB_CI_SET_VISIBILITY_BREAK(ci);
	ci->stack[1] = mrb_nil_value();
	auto ret = mrb_exec_irep(_state, mrb_top_self(_state), proc);
}