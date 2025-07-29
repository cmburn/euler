/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#include "euler/game/state.h"

#include <fstream>
#include <mutex>
#include <semaphore>

#include <mruby/proc.h>
/* mruby/proc.h has to come before mruby/internal.h */
#include <mruby/compile.h>
#include <mruby/internal.h>
#include <mruby/presym.h>

#include "euler/game/ext.h"
#include "euler/graphics/ext.h"
#include "euler/gui/ext.h"
#include "euler/physics/ext.h"
#include "euler/util/ext.h"
#include "euler/util/storage.h"
#include "euler/util/thread.h"
#include "euler/vulkan/ext.h"

static auto state_count = std::binary_semaphore(0);
static std::thread::id main_thread_id;

static constexpr SDL_InitFlags SDL_FLAGS = //
    SDL_INIT_AUDIO			   //
    | SDL_INIT_VIDEO			   //
    | SDL_INIT_EVENTS			   //
    | SDL_INIT_GAMEPAD			   //
    ;

euler::game::State::State(const util::Config &config)
    : _config(config)
{
	_log = make_object<util::Logger>(config.log_level, config.progname);
	_log->info("Creating state");
	if (!state_count.try_acquire())
		_log->critical("Multiple states are not yet supported");
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
	log()->info("Initializing state with {} threads", _config.num_threads);
	static std::once_flag once;
	std::call_once(once, [&]() {
		log()->info("Initializing global state...");
		main_thread_id = std::this_thread::get_id();
		log()->info("Initializing global storage...");
		init_fs(_config.progname.c_str());
		log()->info("Initializing global SDL...");
		SDL_Init(SDL_FLAGS);
		log()->info("Global initialization complete");
	});

	log()->info("Initializing interpreter");
	_state = mrb_open();
	if (_state == nullptr) {
		_log->error("Failed to initialize mruby state");
		return false;
	}
	const auto weak = util::WeakReference(this);
	_state->ud = util::unsafe_cast<void *>(weak);
	log()->info("Initializing core modules");
	/* TODO: proper physfs limiting for module load */
	const auto self = util::Reference(this);
	util::init_util(self);
	vulkan::init_vulkan(self);
	graphics::init_graphics(self);
	gui::init_gui(self);
	physics::init_physics(self);
	init_game(self);
	_log->debug("Core modules initialized");
	return true;
}

bool
euler::game::State::load(std::string_view path)
{
	if (!std::filesystem::exists(path))
		_log->critical("Entry file '{}' does not exist", path);

	log()->info("Loading entry file '{}'", path);
	std::ifstream file(path.data());
	if (!file.is_open()) {
		log()->error("Failed to open entry file '{}'", path);
		return false;
	}
	// if (!require(path)) {
	// 	log()->error("Failed to load entry file '{}'", path);
	// 	return false;
	// }
	log()->info("Entry file '{}' loaded successfully", path);
	return true;
}

static mrb_sym
sdl_event_sym(mrb_state *mrb, SDL_Event &e)
{
	switch (static_cast<SDL_EventType>(e.type)) {
	case SDL_EVENT_QUIT: return MRB_SYM(quit);
	case SDL_EVENT_TERMINATING: return MRB_SYM(terminating);
	case SDL_EVENT_LOW_MEMORY: return MRB_SYM(low_memory);
	case SDL_EVENT_WILL_ENTER_BACKGROUND:
		return MRB_SYM(will_enter_background);
	case SDL_EVENT_DID_ENTER_BACKGROUND:
		return MRB_SYM(did_enter_background);
	case SDL_EVENT_WILL_ENTER_FOREGROUND:
		return MRB_SYM(will_enter_foreground);
	case SDL_EVENT_DID_ENTER_FOREGROUND:
		return MRB_SYM(did_enter_foreground);
	case SDL_EVENT_LOCALE_CHANGED: return MRB_SYM(locale_changed);
	case SDL_EVENT_SYSTEM_THEME_CHANGED:
		return MRB_SYM(system_theme_changed);
	case SDL_EVENT_DISPLAY_ORIENTATION: return MRB_SYM(display_orientation);
	case SDL_EVENT_DISPLAY_ADDED: return MRB_SYM(display_added);
	case SDL_EVENT_DISPLAY_REMOVED: return MRB_SYM(display_removed);
	case SDL_EVENT_DISPLAY_MOVED: return MRB_SYM(display_moved);
	case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED:
		return MRB_SYM(display_desktop_mode_changed);
	case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED:
		return MRB_SYM(display_current_mode_changed);
	case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
		return MRB_SYM(display_content_scale_changed);
	case SDL_EVENT_WINDOW_SHOWN: return MRB_SYM(window_shown);
	case SDL_EVENT_WINDOW_HIDDEN: return MRB_SYM(window_hidden);
	case SDL_EVENT_WINDOW_EXPOSED: return MRB_SYM(window_exposed);
	case SDL_EVENT_WINDOW_MOVED: return MRB_SYM(window_moved);
	case SDL_EVENT_WINDOW_RESIZED: return MRB_SYM(window_resized);
	case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
		return MRB_SYM(window_pixel_size_changed);
	case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
		return MRB_SYM(window_metal_view_resized);
	case SDL_EVENT_WINDOW_MINIMIZED: return MRB_SYM(window_minimized);
	case SDL_EVENT_WINDOW_MAXIMIZED: return MRB_SYM(window_maximized);
	case SDL_EVENT_WINDOW_RESTORED: return MRB_SYM(window_restored);
	case SDL_EVENT_WINDOW_MOUSE_ENTER: return MRB_SYM(window_mouse_enter);
	case SDL_EVENT_WINDOW_MOUSE_LEAVE: return MRB_SYM(window_mouse_leave);
	case SDL_EVENT_WINDOW_FOCUS_GAINED: return MRB_SYM(window_focus_gained);
	case SDL_EVENT_WINDOW_FOCUS_LOST: return MRB_SYM(window_focus_lost);
	case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
		return MRB_SYM(window_close_requested);
	case SDL_EVENT_WINDOW_HIT_TEST: return MRB_SYM(window_hit_test);
	case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
		return MRB_SYM(window_iccprof_changed);
	case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
		return MRB_SYM(window_display_changed);
	case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
		return MRB_SYM(window_display_scale_changed);
	case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
		return MRB_SYM(window_safe_area_changed);
	case SDL_EVENT_WINDOW_OCCLUDED: return MRB_SYM(window_occluded);
	case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
		return MRB_SYM(window_enter_fullscreen);
	case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
		return MRB_SYM(window_leave_fullscreen);
	case SDL_EVENT_WINDOW_DESTROYED: return MRB_SYM(window_destroyed);
	case SDL_EVENT_WINDOW_HDR_STATE_CHANGED:
		return MRB_SYM(window_hdr_state_changed);
	case SDL_EVENT_KEY_DOWN: return MRB_SYM(key_down);
	case SDL_EVENT_KEY_UP: return MRB_SYM(key_up);
	case SDL_EVENT_TEXT_EDITING: return MRB_SYM(text_editing);
	case SDL_EVENT_TEXT_INPUT: return MRB_SYM(text_input);
	case SDL_EVENT_KEYMAP_CHANGED: return MRB_SYM(keymap_changed);
	case SDL_EVENT_KEYBOARD_ADDED: return MRB_SYM(keyboard_added);
	case SDL_EVENT_KEYBOARD_REMOVED: return MRB_SYM(keyboard_removed);
	case SDL_EVENT_TEXT_EDITING_CANDIDATES:
		return MRB_SYM(text_editing_candidates);
	case SDL_EVENT_MOUSE_MOTION: return MRB_SYM(mouse_motion);
	case SDL_EVENT_MOUSE_BUTTON_DOWN: return MRB_SYM(mouse_button_down);
	case SDL_EVENT_MOUSE_BUTTON_UP: return MRB_SYM(mouse_button_up);
	case SDL_EVENT_MOUSE_WHEEL: return MRB_SYM(mouse_wheel);
	case SDL_EVENT_MOUSE_ADDED: return MRB_SYM(mouse_added);
	case SDL_EVENT_MOUSE_REMOVED: return MRB_SYM(mouse_removed);
	case SDL_EVENT_JOYSTICK_AXIS_MOTION:
		return MRB_SYM(joystick_axis_motion);
	case SDL_EVENT_JOYSTICK_BALL_MOTION:
		return MRB_SYM(joystick_ball_motion);
	case SDL_EVENT_JOYSTICK_HAT_MOTION: return MRB_SYM(joystick_hat_motion);
	case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
		return MRB_SYM(joystick_button_down);
	case SDL_EVENT_JOYSTICK_BUTTON_UP: return MRB_SYM(joystick_button_up);
	case SDL_EVENT_JOYSTICK_ADDED: return MRB_SYM(joystick_added);
	case SDL_EVENT_JOYSTICK_REMOVED: return MRB_SYM(joystick_removed);
	case SDL_EVENT_JOYSTICK_BATTERY_UPDATED:
		return MRB_SYM(joystick_battery_updated);
	case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE:
		return MRB_SYM(joystick_update_complete);
	case SDL_EVENT_GAMEPAD_AXIS_MOTION: return MRB_SYM(gamepad_axis_motion);
	case SDL_EVENT_GAMEPAD_BUTTON_DOWN: return MRB_SYM(gamepad_button_down);
	case SDL_EVENT_GAMEPAD_BUTTON_UP: return MRB_SYM(gamepad_button_up);
	case SDL_EVENT_GAMEPAD_ADDED: return MRB_SYM(gamepad_added);
	case SDL_EVENT_GAMEPAD_REMOVED: return MRB_SYM(gamepad_removed);
	case SDL_EVENT_GAMEPAD_REMAPPED: return MRB_SYM(gamepad_remapped);
	case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
		return MRB_SYM(gamepad_touchpad_down);
	case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
		return MRB_SYM(gamepad_touchpad_motion);
	case SDL_EVENT_GAMEPAD_TOUCHPAD_UP: return MRB_SYM(gamepad_touchpad_up);
	case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
		return MRB_SYM(gamepad_sensor_update);
	case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
		return MRB_SYM(gamepad_update_complete);
	case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
		return MRB_SYM(gamepad_steam_handle_updated);
	case SDL_EVENT_FINGER_DOWN: return MRB_SYM(finger_down);
	case SDL_EVENT_FINGER_UP: return MRB_SYM(finger_up);
	case SDL_EVENT_FINGER_MOTION: return MRB_SYM(finger_motion);
	case SDL_EVENT_FINGER_CANCELED: return MRB_SYM(finger_canceled);
	case SDL_EVENT_CLIPBOARD_UPDATE: return MRB_SYM(clipboard_update);
	case SDL_EVENT_DROP_FILE: return MRB_SYM(drop_file);
	case SDL_EVENT_DROP_TEXT: return MRB_SYM(drop_text);
	case SDL_EVENT_DROP_BEGIN: return MRB_SYM(drop_begin);
	case SDL_EVENT_DROP_COMPLETE: return MRB_SYM(drop_complete);
	case SDL_EVENT_DROP_POSITION: return MRB_SYM(drop_position);
	case SDL_EVENT_AUDIO_DEVICE_ADDED: return MRB_SYM(audio_device_added);
	case SDL_EVENT_AUDIO_DEVICE_REMOVED:
		return MRB_SYM(audio_device_removed);
	case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
		return MRB_SYM(audio_device_format_changed);
	case SDL_EVENT_SENSOR_UPDATE: return MRB_SYM(sensor_update);
	case SDL_EVENT_PEN_PROXIMITY_IN: return MRB_SYM(pen_proximity_in);
	case SDL_EVENT_PEN_PROXIMITY_OUT: return MRB_SYM(pen_proximity_out);
	case SDL_EVENT_PEN_DOWN: return MRB_SYM(pen_down);
	case SDL_EVENT_PEN_UP: return MRB_SYM(pen_up);
	case SDL_EVENT_PEN_BUTTON_DOWN: return MRB_SYM(pen_button_down);
	case SDL_EVENT_PEN_BUTTON_UP: return MRB_SYM(pen_button_up);
	case SDL_EVENT_PEN_MOTION: return MRB_SYM(pen_motion);
	case SDL_EVENT_PEN_AXIS: return MRB_SYM(pen_axis);
	case SDL_EVENT_CAMERA_DEVICE_ADDED: return MRB_SYM(camera_device_added);
	case SDL_EVENT_CAMERA_DEVICE_REMOVED:
		return MRB_SYM(camera_device_removed);
	case SDL_EVENT_CAMERA_DEVICE_APPROVED:
		return MRB_SYM(camera_device_approved);
	case SDL_EVENT_CAMERA_DEVICE_DENIED:
		return MRB_SYM(camera_device_denied);
	case SDL_EVENT_RENDER_TARGETS_RESET:
		return MRB_SYM(render_targets_reset);
	case SDL_EVENT_RENDER_DEVICE_RESET: return MRB_SYM(render_device_reset);
	case SDL_EVENT_RENDER_DEVICE_LOST: return MRB_SYM(render_device_lost);
	default:
		return 0;
	}
}

bool
euler::game::State::handle_event(const SDL_Event &e)
{
	// log->error("Unknown SDL event type: {}", e.type);
	[[maybe_unused]] auto guard = _window->input_guard();

}

bool
euler::game::State::load_text(std::string_view source, std::string_view data)
{
	auto ci = _state->c->ci > _state->c->cibase ? &_state->c->ci[-1]
						    : _state->c->cibase;
	auto scope = ci->proc;
	auto ctx = mrb_ccontext_new(_state);
	ctx->capture_errors = true;
	if (scope != nullptr && MRB_PROC_CFUNC_P(scope)) ctx->upper = nullptr;
	else ctx->upper = scope;
	const auto p = mrb_parse_nstring(_state, data.data(), data.size(), ctx);
	if (p == nullptr) {
		log()->error("Failed to parse {}, out of memory!", source);
		mrb_ccontext_free(_state, ctx);
		return false;
	}
	if (p->nerr > 0) {
		for (int i = 0; i < p->nerr; i++) {
			const auto &[lineno, column, message]
			    = p->error_buffer[i];
			_log->critical("Error in {}: {} at line {}: {}", source,
			    message, lineno, column);
		}
		mrb_ccontext_free(_state, ctx);
		mrb_parser_free(p);
		return false;
	}
	auto proc = mrb_generate_code(_state, p);
	if (proc == nullptr) {
		_log->critical("Failed to generate code for {}", source);
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
	const auto ret = mrb_exec_irep(_state, mrb_top_self(_state), proc);
	if (_state->exc) {
		_log->critical("Failed to execute {}: {}", source,
		    mrb_obj_value(_state->exc));
		_state->exc = nullptr;
		return false;
	}
	return true;
}

bool
euler::game::State::initialize()
{
	return load_core() && load(_config.entry_file);
}

bool
euler::game::State::loop(int &exit_code)
{
	assert(util::is_main_thread());
	SDL_Event e;
	// while (SDL_WaitEvent(&e)) {
	// 	if (e.type == SDL_EVENT_QUIT) {
	// 		_log->info("Received quit event, exiting loop");
	// 		exit_code = 0;
	// 		return false;
	// 	}
	// }
	if (!SDL_WaitEvent(&e)) {
		_log->error("Failed to wait for event: {}", SDL_GetError());
		exit_code = EXIT_FAILURE;
		return false;
	}
	if (e.type == SDL_EVENT_QUIT) {
		_log->info("Received quit event, exiting loop");
		exit_code = 0;
		return false;
	}
	_log->trace("Processing event: {}", e.type);
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
	if (!load_text(path, data)) {
		_log->error("Failed to load module '{}'", path);
		return false;
	}
	_loaded_modules.insert(path.data());
	_log->info("Module '{}' loaded successfully", path);

	return true;
}