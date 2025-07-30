/* SPDX-License-Identifier: ISC */

#include <array>
#include <cassert>
#include <stdexcept>

#include "euler/ext/sdl.h"

#include <mruby/array.h>
#include <mruby/hash.h>
#include <mruby/presym.h>

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
	default: return 0;
	}
}

#define ADD_ENTRY(key, value)                                                  \
	mrb_hash_set(mrb, hash, mrb_symbol_value(MRB_SYM(key)), value)

static void
display_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(display_id, mrb_int_value(mrb, e.display.displayID));
	auto array = mrb_ary_new(mrb);
	mrb_ary_push(mrb, array, mrb_int_value(mrb, e.display.data1));
	mrb_ary_push(mrb, array, mrb_int_value(mrb, e.display.data2));
	// mrb_hash_set(mrb, hash, mrb_symbol_value(MRB_SYM(data)), array);
	ADD_ENTRY(data, array);
}

static void
window_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.window.windowID));
	auto array = mrb_ary_new(mrb);
	mrb_ary_push(mrb, array, mrb_int_value(mrb, e.window.data1));
	mrb_ary_push(mrb, array, mrb_int_value(mrb, e.window.data2));
	ADD_ENTRY(data, array);
}

static void
keyboard_device_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	mrb_hash_set(mrb, hash, mrb_symbol_value(MRB_SYM(which)),
	    mrb_int_value(mrb, e.kdevice.which));
}

static void
keyboard_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.key.windowID));
	ADD_ENTRY(which, mrb_int_value(mrb, e.key.which));
	ADD_ENTRY(scancode, mrb_int_value(mrb, e.key.scancode));
	ADD_ENTRY(key, mrb_int_value(mrb, e.key.key));
	ADD_ENTRY(mod, mrb_int_value(mrb, e.key.mod));
	ADD_ENTRY(raw, mrb_int_value(mrb, e.key.raw));
	ADD_ENTRY(down, mrb_bool_value(e.key.down));
	ADD_ENTRY(repeat, mrb_bool_value(e.key.repeat));
}

static void
text_editing_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.edit.windowID));
	ADD_ENTRY(text, mrb_str_new_cstr(mrb, e.edit.text));
	ADD_ENTRY(start, mrb_int_value(mrb, e.edit.start));
	ADD_ENTRY(length, mrb_int_value(mrb, e.edit.length));
}

static void
text_editing_candidates_event(mrb_state *mrb, const SDL_Event &e,
    mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.edit_candidates.windowID));
	auto array = mrb_ary_new(mrb);
	mrb_value selected = mrb_nil_value();
	for (int i = 0; i < e.edit_candidates.num_candidates; ++i) {
		const auto cstr
		    = mrb_str_new_cstr(mrb, e.edit_candidates.candidates[i]);
		if (i == e.edit_candidates.selected_candidate) selected = cstr;
		mrb_ary_push(mrb, array, cstr);
	}
	ADD_ENTRY(candidates, array);
	ADD_ENTRY(selected_candidate, selected);
	ADD_ENTRY(horizontal, mrb_bool_value(e.edit_candidates.horizontal));
}

static void
text_input_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.text.windowID));
	ADD_ENTRY(text, mrb_str_new_cstr(mrb, e.text.text));
}

static void
mouse_device_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.mdevice.which));
}

static void
mouse_motion_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.motion.windowID));
	ADD_ENTRY(which, mrb_int_value(mrb, e.motion.which));
	ADD_ENTRY(x, mrb_float_value(mrb, e.motion.x));
	ADD_ENTRY(y, mrb_float_value(mrb, e.motion.y));
	ADD_ENTRY(xrel, mrb_float_value(mrb, e.motion.xrel));
	ADD_ENTRY(yrel, mrb_float_value(mrb, e.motion.yrel));
}

static void
mouse_button_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.button.windowID));
	ADD_ENTRY(which, mrb_int_value(mrb, e.button.which));
	ADD_ENTRY(button, mrb_int_value(mrb, e.button.button));
	ADD_ENTRY(down, mrb_bool_value(e.button.down));
	ADD_ENTRY(clicks, mrb_int_value(mrb, e.button.clicks));
	ADD_ENTRY(x, mrb_float_value(mrb, e.button.x));
	ADD_ENTRY(y, mrb_float_value(mrb, e.button.y));
}

static void
mouse_wheel_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.wheel.windowID));
	ADD_ENTRY(which, mrb_int_value(mrb, e.wheel.which));
	ADD_ENTRY(x, mrb_float_value(mrb, e.wheel.x));
	ADD_ENTRY(y, mrb_float_value(mrb, e.wheel.y));
	assert(e.wheel.direction == SDL_MOUSEWHEEL_NORMAL
	    || e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED);
	switch (e.wheel.direction) {
	case SDL_MOUSEWHEEL_NORMAL:
		ADD_ENTRY(direction, mrb_symbol_value(MRB_SYM(normal)));
		break;
	case SDL_MOUSEWHEEL_FLIPPED:
		ADD_ENTRY(direction, mrb_symbol_value(MRB_SYM(flipped)));
		break;
	default: assert(false);
	}
	ADD_ENTRY(mouse_x, mrb_float_value(mrb, e.wheel.mouse_x));
	ADD_ENTRY(mouse_y, mrb_float_value(mrb, e.wheel.mouse_y));
	ADD_ENTRY(integer_x, mrb_int_value(mrb, e.wheel.integer_x));
	ADD_ENTRY(integer_y, mrb_int_value(mrb, e.wheel.integer_y));
}

static void
joy_axis_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.jaxis.which));
	ADD_ENTRY(axis, mrb_int_value(mrb, e.jaxis.axis));
	ADD_ENTRY(value, mrb_int_value(mrb, e.jaxis.value));
}

static void
joy_ball_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.jball.which));
	ADD_ENTRY(ball, mrb_int_value(mrb, e.jball.ball));
	ADD_ENTRY(xrel, mrb_int_value(mrb, e.jball.xrel));
	ADD_ENTRY(yrel, mrb_int_value(mrb, e.jball.yrel));
}

static void
joy_hat_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.jhat.which));
	ADD_ENTRY(hat, mrb_int_value(mrb, e.jhat.hat));
	ADD_ENTRY(value, mrb_int_value(mrb, e.jhat.value));
}

static void
joy_button_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.jbutton.which));
	ADD_ENTRY(button, mrb_int_value(mrb, e.jbutton.button));
	ADD_ENTRY(down, mrb_bool_value(e.jbutton.down));
}

static void
joy_device_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.jdevice.which));
}

static void
joy_battery_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.jbattery.which));
	switch (e.jbattery.state) {
	case SDL_POWERSTATE_ERROR:
		ADD_ENTRY(state, mrb_symbol_value(MRB_SYM(error)));
		break;
	case SDL_POWERSTATE_ON_BATTERY:
		ADD_ENTRY(state, mrb_symbol_value(MRB_SYM(on_battery)));
		break;
	case SDL_POWERSTATE_NO_BATTERY:
		ADD_ENTRY(state, mrb_symbol_value(MRB_SYM(no_battery)));
		break;
	case SDL_POWERSTATE_CHARGING:
		ADD_ENTRY(state, mrb_symbol_value(MRB_SYM(charging)));
		break;
	case SDL_POWERSTATE_CHARGED:
		ADD_ENTRY(state, mrb_symbol_value(MRB_SYM(charged)));
		break;
	default: ADD_ENTRY(state, mrb_symbol_value(MRB_SYM(unknown)));
	}
	ADD_ENTRY(percent, mrb_int_value(mrb, e.jbattery.percent));
}

static void
gamepad_axis_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.gaxis.which));
	ADD_ENTRY(axis, mrb_int_value(mrb, e.gaxis.axis));
	ADD_ENTRY(value, mrb_int_value(mrb, e.gaxis.value));
}

static void
gamepad_button_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.gbutton.which));
	ADD_ENTRY(button, mrb_int_value(mrb, e.gbutton.button));
	ADD_ENTRY(down, mrb_bool_value(e.gbutton.down));
}

static void
gamepad_device_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.gdevice.which));
}

static void
gamepad_touchpad_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.gtouchpad.which));
	ADD_ENTRY(touchpad, mrb_int_value(mrb, e.gtouchpad.touchpad));
	ADD_ENTRY(finger, mrb_int_value(mrb, e.gtouchpad.finger));
	ADD_ENTRY(x, mrb_float_value(mrb, e.gtouchpad.x));
	ADD_ENTRY(y, mrb_float_value(mrb, e.gtouchpad.y));
	ADD_ENTRY(pressure, mrb_float_value(mrb, e.gtouchpad.pressure));
}

static void
gamepad_sensor_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.gsensor.which));
	ADD_ENTRY(sensor, mrb_int_value(mrb, e.gsensor.sensor));
	auto array = mrb_ary_new(mrb);
	for (int i = 0; i < 3; ++i) {
		const auto num = mrb_float_value(mrb, e.gsensor.data[i]);
		mrb_ary_push(mrb, array, num);
	}
	ADD_ENTRY(data, array);
	ADD_ENTRY(sensor_timestamp,
	    mrb_int_value(mrb, e.gsensor.sensor_timestamp));
}

static void
audio_device_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.adevice.which));
	ADD_ENTRY(recording, mrb_bool_value(e.adevice.recording));
}

static void
camera_device_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.cdevice.which));
}

static void
render_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.render.windowID));
}

static void
touch_finger_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(touch_id, mrb_int_value(mrb, e.tfinger.touchID));
	ADD_ENTRY(finger_id, mrb_int_value(mrb, e.tfinger.fingerID));
	ADD_ENTRY(x, mrb_float_value(mrb, e.tfinger.x));
	ADD_ENTRY(y, mrb_float_value(mrb, e.tfinger.y));
	ADD_ENTRY(dx, mrb_float_value(mrb, e.tfinger.dx));
	ADD_ENTRY(dy, mrb_float_value(mrb, e.tfinger.dy));
	ADD_ENTRY(pressure, mrb_float_value(mrb, e.tfinger.pressure));
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.tfinger.windowID));
}

static void
pen_proximity_event(mrb_state *mrb, const SDL_Event &e, const mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.pproximity.windowID));
	ADD_ENTRY(which, mrb_int_value(mrb, e.pproximity.which));
}

static void
pen_motion_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.pmotion.windowID));
	ADD_ENTRY(which, mrb_int_value(mrb, e.pmotion.which));
	ADD_ENTRY(x, mrb_float_value(mrb, e.pmotion.x));
	ADD_ENTRY(y, mrb_float_value(mrb, e.pmotion.y));
}

#define CONCAT2(a, b) a##b
#define CONCAT(a, b) CONCAT2(a, b)

static mrb_value
pen_input_flags(mrb_state *mrb, SDL_PenInputFlags state)
{
	auto input_hash = mrb_hash_new(mrb);
	if (state & SDL_PEN_INPUT_DOWN) {
		mrb_hash_set(mrb, input_hash, mrb_symbol_value(MRB_SYM(down)),
		    mrb_bool_value(true));
	}
	if (state & SDL_PEN_INPUT_BUTTON_1) {
		mrb_hash_set(mrb, input_hash,
		    mrb_symbol_value(MRB_SYM(button_1)), mrb_bool_value(true));
	}
	if (state & SDL_PEN_INPUT_BUTTON_2) {
		mrb_hash_set(mrb, input_hash,
		    mrb_symbol_value(MRB_SYM(button_2)), mrb_bool_value(true));
	}
	if (state & SDL_PEN_INPUT_BUTTON_3) {
		mrb_hash_set(mrb, input_hash,
		    mrb_symbol_value(MRB_SYM(button_3)), mrb_bool_value(true));
	}
	if (state & SDL_PEN_INPUT_BUTTON_4) {
		mrb_hash_set(mrb, input_hash,
		    mrb_symbol_value(MRB_SYM(button_4)), mrb_bool_value(true));
	}
	if (state & SDL_PEN_INPUT_BUTTON_5) {
		mrb_hash_set(mrb, input_hash,
		    mrb_symbol_value(MRB_SYM(button_5)), mrb_bool_value(true));
	}
	if (state & SDL_PEN_INPUT_ERASER_TIP) {
		mrb_hash_set(mrb, input_hash,
		    mrb_symbol_value(MRB_SYM(eraser_tip)),
		    mrb_bool_value(true));
	}
	return input_hash;
}

static void
pen_touch_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.ptouch.windowID));
	ADD_ENTRY(which, mrb_int_value(mrb, e.ptouch.which));
	ADD_ENTRY(pen_state, pen_input_flags(mrb, e.ptouch.pen_state));
	ADD_ENTRY(x, mrb_float_value(mrb, e.ptouch.x));
	ADD_ENTRY(y, mrb_float_value(mrb, e.ptouch.y));
	ADD_ENTRY(eraser, mrb_bool_value(e.ptouch.eraser));
	ADD_ENTRY(down, mrb_bool_value(e.ptouch.down));
}

static void
pen_button_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.pbutton.windowID));
	ADD_ENTRY(which, mrb_int_value(mrb, e.pbutton.which));
	ADD_ENTRY(pen_state, pen_input_flags(mrb, e.pbutton.pen_state));
	ADD_ENTRY(x, mrb_float_value(mrb, e.pbutton.x));
	ADD_ENTRY(y, mrb_float_value(mrb, e.pbutton.y));
	ADD_ENTRY(button, mrb_int_value(mrb, e.pbutton.button));
	ADD_ENTRY(down, mrb_bool_value(e.pbutton.down));
}

static mrb_sym
pen_axis_sym(mrb_state *mrb, const SDL_Event &e)
{
	switch (e.paxis.axis) {
	case SDL_PEN_AXIS_PRESSURE: return MRB_SYM(pressure);
	case SDL_PEN_AXIS_XTILT: return MRB_SYM(xtilt);
	case SDL_PEN_AXIS_YTILT: return MRB_SYM(ytilt);
	case SDL_PEN_AXIS_DISTANCE: return MRB_SYM(distance);
	case SDL_PEN_AXIS_ROTATION: return MRB_SYM(rotation);
	case SDL_PEN_AXIS_SLIDER: return MRB_SYM(slider);
	case SDL_PEN_AXIS_TANGENTIAL_PRESSURE:
		return MRB_SYM(tangential_pressure);
	case SDL_PEN_AXIS_COUNT: return MRB_SYM(count);
	default: return 0;
	}
}

static void
pen_axis_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.paxis.windowID));
	ADD_ENTRY(which, mrb_int_value(mrb, e.paxis.which));
	ADD_ENTRY(axis, mrb_int_value(mrb, e.paxis.axis));
	ADD_ENTRY(pen_state, pen_input_flags(mrb, e.paxis.pen_state));
	ADD_ENTRY(x, mrb_float_value(mrb, e.paxis.x));
	ADD_ENTRY(y, mrb_float_value(mrb, e.paxis.y));
	ADD_ENTRY(axis, mrb_symbol_value(pen_axis_sym(mrb, e)));
	ADD_ENTRY(value, mrb_float_value(mrb, e.paxis.value));
}

static void
drop_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(window_id, mrb_int_value(mrb, e.drop.windowID));
	ADD_ENTRY(x, mrb_float_value(mrb, e.drop.x));
	ADD_ENTRY(y, mrb_float_value(mrb, e.drop.y));
	/* TODO */
	ADD_ENTRY(file, mrb_str_new_cstr(mrb, e.drop.source));
	ADD_ENTRY(text, mrb_str_new_cstr(mrb, e.drop.data));
}

static void
clipboard_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(owner, mrb_bool_value(e.clipboard.owner));
	auto mime_types = mrb_ary_new(mrb);
	for (Sint32 i = 0; i < e.clipboard.num_mime_types; ++i) {
		const auto mime_type
		    = mrb_str_new_cstr(mrb, e.clipboard.mime_types[i]);
		mrb_ary_push(mrb, mime_types, mime_type);
	}
	ADD_ENTRY(mime_types, mime_types);
}

static void
sensor_event(mrb_state *mrb, const SDL_Event &e, mrb_value hash)
{
	ADD_ENTRY(which, mrb_int_value(mrb, e.sensor.which));
	const auto array = mrb_ary_new(mrb);
	for (int i = 0; i < 3; ++i) {
		const auto num = mrb_float_value(mrb, e.sensor.data[i]);
		mrb_ary_push(mrb, array, num);
	}
	ADD_ENTRY(data, array);
	ADD_ENTRY(sensor_timestamp,
	    mrb_int_value(mrb, e.sensor.sensor_timestamp));
}



mrb_value
euler::ext::to_mrb_value(mrb_state *mrb, const SDL_Event &e)
{
	mrb_sym sym = sdl_event_sym(mrb, const_cast<SDL_Event &>(e));
	if (sym == 0) return mrb_nil_value();
	const auto hash = mrb_hash_new(mrb);
	const auto timestamp = e.common.timestamp;
	mrb_hash_set(mrb, hash, mrb_symbol_value(MRB_SYM__type),
	    mrb_symbol_value(sym));
	mrb_hash_set(mrb, hash, mrb_symbol_value(MRB_SYM(timestamp)),
	    mrb_int_value(mrb, timestamp));
}
#undef ADD_VALUE
