/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#include "euler/app/state.h"

#include <fstream>
#include <mutex>
#include <semaphore>

#include <mruby.h>
#include <SDL3/SDL_events.h>
#include <mruby/array.h>
#include <mruby/compile.h>
#include <mruby/error.h>
#include <mruby/presym.h>
#include <mruby/proc.h>
#include <mruby/string.h>
#include <mruby/throw.h>
#include <mruby/variable.h>

#include "euler/app/event.h"
#include "euler/app/game_ext.h"
#include "euler/app/graphics_ext.h"
#include "euler/app/gui_ext.h"
#include "euler/app/util_ext.h"
#include "euler/app/vulkan_ext.h"
#include "euler/app/window.h"

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

static void assert_state_integrity(mrb_state *mrb)
{
	static mrb_state *first_state = nullptr;
	if (first_state == nullptr) first_state = mrb;
	if (mrb == first_state) return;
	fprintf(stderr, "mrb_state has changed!!\n");
	abort();
}

static std::string
read_exception(mrb_state *mrb, RObject *exc)
{
	// TODO: backtrace support
	const auto obj = mrb_obj_value(exc);
	const auto message = mrb_funcall(mrb, obj, "message", 0);
	if (!mrb_string_p(message)) return "Exception message is not a string";
	return std::string(RSTRING_PTR(message), RSTRING_LEN(message));
}

mrb_value
euler::app::State::gv_state()
{
	const auto value = mrb_gv_get(_mrb, MRB_GVSYM(state));
	return value;
}

void
euler::app::State::set_ivs()
{
	if (!mrb_nil_p(_attributes.system))
		mrb_gc_unregister(_mrb, _attributes.system);
	const auto system_data = Data_Wrap_Struct(_mrb, _euler.game.system,
	    &SYSTEM_TYPE, system().wrap());
	_attributes.system = mrb_obj_value(system_data);
	mrb_gc_register(_mrb, _attributes.system);
	mrb_iv_set(_mrb, _attributes.self, MRB_IVSYM(system),
	    _attributes.system);
	if (!mrb_nil_p(_attributes.log))
		mrb_gc_unregister(_mrb, _attributes.log);
	const auto log_data = Data_Wrap_Struct(_mrb, _euler.util.logger.klass,
	    &LOGGER_TYPE, log().wrap());
	_attributes.log = mrb_obj_value(log_data);
	mrb_gc_register(_mrb, _attributes.log);
	mrb_iv_set(_mrb, _attributes.self, MRB_IVSYM(log), _attributes.log);
}

void
euler::app::State::assert_state() const
{
	if (_mrb != nullptr && _mrb->c != nullptr) return;
	log()->error("Corrupted state");
}

euler::util::MRubyException
euler::app::State::make_exception(RObject *exc) const
{
	return util::MRubyException(read_exception(_mrb, exc));
}

euler::app::State::State(const util::Config &config)
    : _config(config)
{
	_log = util::make_reference<util::Logger>(config.progname, "app",
	    config.log_level);
	_log->debug("Creating state");
	if (!state_count.try_acquire())
		_log->fatal("Multiple states are not yet supported");
}

bool
euler::app::State::verify_gv_state()
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
			log()->debug(                                          \
			    "Found optional method '" #NAME "' for $state");   \
		} else {                                                       \
			log()->debug(                                          \
			    "No '" #NAME "' method found for $state");         \
		}                                                              \
	} while (0)
	ASSERT_EXISTS(input);
	CHECK_EXISTS(update);
	CHECK_EXISTS(load);
	CHECK_EXISTS(draw);
	CHECK_EXISTS(quit);
#undef ASSERT_EXISTS
#undef CHECK_EXISTS
	return true;
}

// ReSharper disable once CppMemberFunctionMayBeConst
bool
euler::app::State::app_update(const float dt)
{
	assert_state_integrity(_mrb);
	assert(_methods.update);
	const auto arg = mrb_float_value(_mrb, dt);
	assert_state();
	mrb_funcall_id(_mrb, _attributes.self, MRB_SYM(update), 1, arg);
	assert_state();
	if (_mrb->exc != nullptr) {
		log()->error("exc in update: {}", exception_string().value());
	}
	return _mrb->exc == nullptr;
}

bool
euler::app::State::app_input([[maybe_unused]] const SDL_Event &event)
{
	assert_state_integrity(_mrb);
	return true;
#if 0
	if (!_methods.input)
		return static_cast<SDL_EventType>(event.type) == SDL_EVENT_QUIT;
	try {
		const auto arg = sdl_event_to_mrb(util::Reference(this), event);
		mrb_funcall_id(_mrb, _attributes.self, MRB_SYM(input), 1, arg);
		if (_mrb->exc != nullptr) {
			_log->error("Exception in input: {}",
				exception_string().value());
			_mrb->exc = nullptr;
			return false;
		}
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
	return static_cast<SDL_EventType>(event.type) == SDL_EVENT_QUIT;
#endif
}

bool
euler::app::State::app_draw()
{
	assert_state_integrity(_mrb);
	if (!_methods.draw) return true;
	try {
		assert_state();
		mrb_funcall_id(_mrb, _attributes.self, MRB_SYM(draw), 0);
		assert_state();
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
euler::app::State::app_load()
{
	system()->tick();
	if (!_methods.load) return true;
	try {
		mrb_funcall_id(_mrb, _attributes.self, MRB_SYM(load), 0);
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
	system()->tick();
	return true;
}

bool
euler::app::State::app_quit()
{
	if (!_methods.quit) return true;
	try {
		mrb_funcall_id(_mrb, _attributes.self, MRB_SYM(quit), 0);
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
euler::app::State::exception_string() const
{
	if (_mrb->exc == nullptr) return std::nullopt;
	auto str = mrb_obj_value(_mrb->exc);
	// auto value = mrb_obj_as_string(_mrb, str);
	assert_state();
	auto value = mrb_funcall_id(_mrb, str, MRB_SYM(to_s), 0);
	assert_state();
	if (!mrb_string_p(value)) return std::nullopt;
	const char *ptr = RSTRING_PTR(value);
	const auto len = RSTRING_LEN(value);
	/* ReSharper disable once CppDFALocalValueEscapesFunction */
	return std::string_view(ptr, len);
}

bool
euler::app::State::load_core()
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
	_window = util::make_reference<Window>(_log, _config.progname);
	log()->debug("Initializing Vulkan");
	_renderer = util::make_reference<vulkan::Renderer>();
	_renderer->initialize(_window);

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
	init_app(self);
	const auto data = Data_Wrap_Struct(_mrb, _euler.game.state, &STATE_TYPE,
	    self.wrap());
	_attributes.self = mrb_obj_value(data);
	mrb_gc_register(_mrb, _attributes.self);
	set_ivs();
	_log->debug("Core modules initialized");
	return true;
}

bool
euler::app::State::load_entry(std::string_view path)
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
euler::app::State::load_text(std::string_view source, std::string_view data)
{
	auto cxt = mrb_ccontext_new(_mrb);
	cxt->capture_errors = true;
	auto idx = mrb_gc_arena_save(_mrb);
	mrb_load_nstring_cxt(_mrb, data.data(), data.size(), cxt);
	mrb_gc_arena_restore(_mrb, idx);
	mrb_ccontext_free(_mrb, cxt);
	if (!_mrb->exc) return true;
	_log->fatal("Failed to execute {}: {}", source,
	    exception_string().value_or("Unable to read exception"));
	_mrb->exc = nullptr;
	return false;
}

euler::util::nthread_t
euler::app::State::available_threads() const
{
	static const auto HW_THREADS = std::thread::hardware_concurrency();
	return std::min(_config.num_threads, HW_THREADS);
}

bool
euler::app::State::initialize()
{
	_system = util::make_reference<System>(util::Reference(this));
	log()->info("Initializing state with {} threads", _config.num_threads);
	if (!load_core()) return false;
	if (!load_entry(_config.entry_file)) return false;
	if (!verify_gv_state()) return false;
	_attributes.self = gv_state();
	mrb_gc_register(_mrb, _attributes.self);
	mrb_gc_register(_mrb, _attributes.system);
	mrb_iv_set(_mrb, _attributes.self, MRB_IVSYM(system),
	    _attributes.system);
	if (!app_load()) return false;
	return true;
}

bool
euler::app::State::loop(int &exit_code)
{
	try {
		// return wrap_call<bool>([&]() {
			return update(exit_code); //
		// });
	} catch (const std::exception &e) {
		_log->error("Unhandled exception in loop: {}", e.what());
		exit_code = EXIT_FAILURE;
		return false;
	} catch (...) {
		_log->error("Unknown exception in loop");
		exit_code = EXIT_FAILURE;
		return false;
	}
}

bool
euler::app::State::update(int &exit_code)
{
	assert(util::is_main_thread());
	const auto gc_idx = mrb_gc_arena_save(_mrb);
	system()->tick();
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
euler::app::State::require(const char *path)
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

euler::app::State::~State() { mrb_close(_mrb); }
