/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#include "euler/game/state.h"

#include <fstream>
#include <mutex>
#include <semaphore>

#include <mruby.h>
#include <SDL3/SDL_events.h>
#include <mruby/compile.h>
#include <mruby/presym.h>
#include <mruby/proc.h>
#include <mruby/string.h>
#include <mruby/throw.h>
#include <mruby/variable.h>

#include "euler/game/game_ext.h"
#include "euler/game/graphics_ext.h"
#include "euler/game/gui_ext.h"
#include "euler/game/physics_ext.h"
#include "euler/game/util_ext.h"
#include "euler/game/vulkan_ext.h"
#include "event.h"
#include "euler/game/window.h"

#include "euler/util/storage.h"
#include "euler/util/thread.h"

static auto state_count = std::binary_semaphore(1);
static std::thread::id main_thread_id;

// static constexpr SDL_InitFlags SDL_FLAGS = //
//     SDL_INIT_AUDIO			   //
//     | SDL_INIT_VIDEO			   //
//     | SDL_INIT_EVENTS			   //
//     | SDL_INIT_GAMEPAD			   //
//     ;

/* ReSharper disable once CppDFAConstantFunctionResult */
static constexpr int
sdl_init_flags()
{
	int flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS
	    | SDL_INIT_GAMEPAD;
#ifdef EULER_USE_JOYSTICK
	flags |= SDL_INIT_JOYSTICK;
#endif
#ifdef EULER_USE_HAPTIC
	flags |= SDL_INIT_HAPTIC;
#endif
#ifdef EULER_USE_SENSOR
	flags |= SDL_INIT_SENSOR;
#endif
#ifdef EULER_USE_CAMERA
	flags |= SDL_INIT_CAMERA;
#endif
	return flags;
}

mrb_value
euler::game::State::gv_state()
{
	return mrb_gv_get(_mrb, MRB_GVSYM(state));
}

euler::game::State::State(const util::Config &config)
    : _config(config)
{
	_log = util::make_reference<util::Logger>(config.progname, "game",
	    config.log_level);
	_log->debug("Creating state");
	if (!state_count.try_acquire())
		_log->fatal("Multiple states are not yet supported");
}

bool
euler::game::State::verify_gv_state()
{
	auto var = gv_state();
	if (mrb_nil_p(var)) {
		log()->error("Global variable '$state' is not defined");
		return false;
	}
	if (!mrb_obj_is_kind_of(_mrb, var, _euler.game.state)) {
		log()->error("Global variable '$state' does not inherit from "
			     "Euler::Game::State");
		return false;
	}
	// There's two required methods: input and update
	// There are also three optional methods: load, draw, quit
#define ASSERT_EXISTS(NAME)                                                    \
	do {                                                                   \
		if (!mrb_respond_to(_mrb, var, MRB_SYM(NAME))) {               \
			log()->error(                                          \
			    "Global variable '$state' does not implement "     \
			    "the '" #NAME "' method");                         \
			return false;                                          \
		}                                                              \
		_methods.NAME = true;                                          \
	} while (0)
#define CHECK_EXISTS(NAME)                                                     \
	do {                                                                   \
		if (mrb_respond_to(_mrb, var, MRB_SYM(NAME))) {                \
			_methods.NAME                                          \
			    = mrb_respond_to(_mrb, var, MRB_SYM(NAME));        \
			log()->debug(                                           \
			    "Found optional method '" #NAME "' for $state");   \
		} else {                                                       \
			log()->debug("No '" #NAME "' method found for $state"); \
		}                                                              \
	} while (0)
	ASSERT_EXISTS(input);
	ASSERT_EXISTS(update);
	CHECK_EXISTS(load);
	CHECK_EXISTS(draw);
	CHECK_EXISTS(quit);
#undef ASSERT_EXISTS
#undef CHECK_EXISTS
	return true;
}

// ReSharper disable once CppMemberFunctionMayBeConst
bool
euler::game::State::app_update(const float dt)
{
	assert(_methods.update);
	const auto arg = mrb_float_value(_mrb, dt);
	mrb_gc_register(_mrb, arg);
	mrb_funcall_id(_mrb, _self_value, MRB_SYM(update), 1, arg);
	if (_mrb->exc != nullptr) {
		_log->error("Exception in update: {}",
		    exception_string().value());
		_mrb->exc = nullptr;
		return false;
	}
	return true;
}

bool
euler::game::State::app_input(const SDL_Event &event)
{
	if (static_cast<SDL_EventType>(event.type) == SDL_EVENT_QUIT)
		return false;
	try {
#if 0
		assert(_methods.input);
		const auto arg = sdl_event_to_mrb(util::Reference(this), event);
		mrb_funcall_id(_mrb, _self_value, MRB_SYM(input), 1, arg);
		if (_mrb->exc != nullptr) {
			_log->error("Exception in input: {}",
			    exception_string().value());
			_mrb->exc = nullptr;
			return false;
		}
#endif
		return true;
	} catch (const std::exception &e) {
		_log->error("Unhandled exception in input: {}", e.what());
		return false;
	} catch (mrb_jmpbuf *e) {
		if (e != (_mrb->jmp)) throw e;
		_log->error("Unhandled mruby exception in input: {}",
		    exception_string().value());
		return false;
	}
}

bool
euler::game::State::app_draw()
{
	if (!_methods.draw) return true;
	try {
		mrb_funcall_id(_mrb, _self_value, MRB_SYM(draw), 0);
		if (_mrb->exc != nullptr) {
			_log->error("Exception in draw: {}",
			    exception_string().value());
			_mrb->exc = nullptr;
			return false;
		}
	} catch (const std::exception &e) {
		_log->error("Unhandled exception in draw: {}", e.what());
		return false;
	} catch (mrb_jmpbuf *e) {
		if (e != (_mrb->jmp)) throw e;
		_log->error("Unhandled mruby exception in draw: {}",
		    exception_string().value());
		return false;
	}
	return true;
}

bool
euler::game::State::app_load()
{
	if (!_methods.load) return true;
	try {
		mrb_funcall_id(_mrb, _self_value, MRB_SYM(load), 0);
		if (_mrb->exc != nullptr) {
			_log->error("Exception in load: {}",
			    exception_string().value());
			_mrb->exc = nullptr;
			return false;
		}
	} catch (const std::exception &e) {
		_log->error("Unhandled exception in load: {}", e.what());
		return false;
	} catch (mrb_jmpbuf *e) {
		if (e != (_mrb->jmp)) throw e;
		_log->error("Unhandled mruby exception in load: {}",
		    exception_string().value());
		return false;
	}
	return true;
}

bool
euler::game::State::app_quit()
{
	if (!_methods.quit) return true;
	try {
		mrb_funcall_id(_mrb, _self_value, MRB_SYM(quit), 0);
		if (_mrb->exc != nullptr) {
			_log->error("Exception in quit: {}",
			    exception_string().value());
			_mrb->exc = nullptr;
			return false;
		}
	} catch (const std::exception &e) {
		_log->error("Unhandled exception in quit: {}", e.what());
		return false;
	} catch (mrb_jmpbuf *e) {
		if (e != (_mrb->jmp)) throw e;
		_log->error("Unhandled mruby exception in quit: {}",
		    exception_string().value());
		return false;
	}
	return true;
}

std::optional<std::string_view>
euler::game::State::exception_string() const
{
	if (_mrb->exc == nullptr) return std::nullopt;
	auto str = mrb_obj_value(_mrb->exc);
	auto value = mrb_obj_as_string(_mrb, str);
	const char *ptr = RSTRING_PTR(value);
	const auto len = RSTRING_LEN(value);
	/* ReSharper disable once CppDFALocalValueEscapesFunction */
	return std::string_view(ptr, len);
}

bool
euler::game::State::load_core()
{
	/**
	 * Have a few threads:
	 * - SDL
	 * - Vulkan
	 * - Game logic/mruby interpreter
	 * - Physics
	 */
	static std::once_flag once;
	std::call_once(once, [&]() {
		log()->debug("Initializing global state...");
		main_thread_id = std::this_thread::get_id();
		log()->debug("Initializing global storage...");
		init_fs(_config.progname.c_str());
		log()->debug("Initializing global SDL...");
		SDL_Init(sdl_init_flags());
		log()->debug("Global initialization complete");
	});
	log()->debug("Creating window");
	_window
	    = util::make_reference<Window>(_log, _config.progname);
	log()->debug("Initializing interpreter");
	_mrb = mrb_open();
	if (_mrb == nullptr) {
		_log->error("Failed to initialize mruby state");
		return false;
	}
	const auto weak = util::WeakReference(this);
	_mrb->ud = util::unsafe_cast<void *>(weak);
	log()->debug("Initializing core modules");
	/* TODO: proper physfs limiting for module load */
	_euler.module = mrb_define_module(_mrb, "Euler");
	auto self = util::Reference(this);
	init_util(self);
	init_vulkan(self);
	init_graphics(self);
	init_gui(self);
	init_physics(self);
	init_game(self);
	auto data = Data_Wrap_Struct(_mrb, _euler.game.state, &STATE_TYPE,
	    self.wrap());
	_self_value = mrb_obj_value(data);
	_log->debug("Core modules initialized");
	return true;
}

bool
euler::game::State::load_entry(std::string_view path)
{
	if (!std::filesystem::exists(path))
		_log->error("Entry file '{}' does not exist", path);

	log()->info("Loading entry file '{}'", path);
	std::ifstream file(path.data());
	if (!file.is_open()) {
		log()->error("Failed to open entry file '{}'", path);
		return false;
	}
	std::string content((std::istreambuf_iterator(file)),
	    std::istreambuf_iterator<char>());
	if (!load_text(path, content)) {
		log()->error("Failed to load entry file '{}'", path);
	}
	log()->debug("Entry file '{}' loaded successfully", path);
	return true;
}

bool
euler::game::State::load_text(std::string_view source, std::string_view data)
{
	auto ci = _mrb->c->ci > _mrb->c->cibase ? &_mrb->c->ci[-1]
						: _mrb->c->cibase;
	auto scope = ci->proc;
	auto ctx = mrb_ccontext_new(_mrb);
	ctx->capture_errors = true;
	if (scope != nullptr && MRB_PROC_CFUNC_P(scope)) ctx->upper = nullptr;
	else ctx->upper = scope;
	const auto p = mrb_parse_nstring(_mrb, data.data(), data.size(), ctx);
	if (p == nullptr) {
		log()->error("Failed to parse {}, out of memory!", source);
		mrb_ccontext_free(_mrb, ctx);
		return false;
	}
	if (p->nerr > 0) {
		for (size_t i = 0; i < p->nerr; i++) {
			const auto &[lineno, column, message]
			    = p->error_buffer[i];
			_log->fatal("Error in {}: {} at line {}: {}", source,
			    message, lineno, column);
		}
		mrb_ccontext_free(_mrb, ctx);
		mrb_parser_free(p);
		return false;
	}
	const auto proc = mrb_generate_code(_mrb, p);
	if (proc == nullptr) {
		_log->fatal("Failed to generate code for {}", source);
		mrb_ccontext_free(_mrb, ctx);
		mrb_parser_free(p);
		return false;
	}

	try {
		mrb_toplevel_run(_mrb, proc);
	} catch (mrb_jmpbuf *e) {
		if (e != (_mrb->jmp)) throw;
		mrb_ccontext_free(_mrb, ctx);
		mrb_parser_free(p);
		assert(_mrb->exc != nullptr);
		_log->fatal("Exception while executing {}: {}", source,
		    exception_string().value());
		return false;
	}

	mrb_ccontext_free(_mrb, ctx);
	mrb_parser_free(p);

	if (_mrb->exc) {
		_log->fatal("Failed to execute {}: {}", source,
		    exception_string().value());
		_mrb->exc = nullptr;
		return false;
	}

	return true;
}

euler::util::nthread_t
euler::game::State::available_threads() const
{
	return std::min(_config.num_threads,
	    std::thread::hardware_concurrency());
}

bool
euler::game::State::initialize()
{
	_system = util::make_reference<System>(util::Reference(this));
	log()->info("Initializing state with {} threads", _config.num_threads);
	if (!load_core()) return false;
	if (!load_entry(_config.entry_file)) return false;
	if (!verify_gv_state()) return false;
	_self_value = gv_state();
	mrb_gc_protect(_mrb, _self_value);
	if (!app_load()) return false;
	_tick = SDL_GetTicks();
	return true;
}
bool
euler::game::State::loop(int &exit_code)
{
	try {
		return update(exit_code);
	} catch (const std::exception &e) {
		_log->error("Unhandled exception in loop: {}", e.what());
		exit_code = EXIT_FAILURE;
		return false;
	} catch (mrb_jmpbuf *e) {
		if (e != (_mrb->jmp)) throw e;
		_log->error("Unhandled mruby exception in loop: {}",
		    exception_string().value());
		exit_code = EXIT_FAILURE;
		return false;
	}
}

bool
euler::game::State::update(int &exit_code)
{
	assert(util::is_main_thread());
	_last_tick = _tick;
	_tick = SDL_GetTicks();
	const auto gc_idx = mrb_gc_arena_save(_mrb);
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		_log->debug("Received event {}", e.type);
		if (e.type == SDL_EVENT_QUIT) {
			_log->info("Received quit event, exiting loop");
			exit_code = 0;
			return false;
		}
		if (_methods.input && !app_input(e)) return false;
	}
	assert(_methods.update);
	if (!app_update(_system->dt())) return false;
	mrb_gc_arena_restore(_mrb, gc_idx);
	if (_methods.draw && !app_draw()) return false;
	return true;
}

/* Kernel#require implementation */
bool
euler::game::State::require(const char *path)
{
	const auto str = std::string(path);
	if (_loaded_modules.contains(str)) {
		_log->debug("Module '{}' already loaded", path);
		return false;
	}
	const auto data = _user_storage->read_file(path);
	if (!load_text(path, data)) {
		_log->error("Failed to load module '{}'", path);
		return false;
	}
	_loaded_modules.insert(std::move(str));
	_log->info("Module '{}' loaded successfully", path);

	return true;
}
