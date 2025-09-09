/* SPDX-License-Identifier: ISC */

#include "euler/game/event.h"

#include <mruby.h>
#include <mruby/array.h>
#include <mruby/object.h>
#include <mruby/variable.h>

static mrb_sym
sdl_event_sym([[maybe_unused]] mrb_state *mrb, const SDL_Event &e)
{
	switch (static_cast<SDL_EventType>(e.type)) {
	case SDL_EVENT_QUIT: return MRB_SYM(quit);
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

static mrb_sym
sdl_scancode_sym(mrb_state *mrb, SDL_Scancode code)
{
	switch (code) {
	case SDL_SCANCODE_0: return mrb_intern_lit(mrb, "0");
	case SDL_SCANCODE_1: return mrb_intern_lit(mrb, "1");
	case SDL_SCANCODE_2: return mrb_intern_lit(mrb, "2");
	case SDL_SCANCODE_3: return mrb_intern_lit(mrb, "3");
	case SDL_SCANCODE_4: return mrb_intern_lit(mrb, "4");
	case SDL_SCANCODE_5: return mrb_intern_lit(mrb, "5");
	case SDL_SCANCODE_6: return mrb_intern_lit(mrb, "6");
	case SDL_SCANCODE_7: return mrb_intern_lit(mrb, "7");
	case SDL_SCANCODE_8: return mrb_intern_lit(mrb, "8");
	case SDL_SCANCODE_9: return mrb_intern_lit(mrb, "9");
	case SDL_SCANCODE_A: return MRB_SYM(a);
	case SDL_SCANCODE_AC_BACK: return MRB_SYM(ac_back);
	case SDL_SCANCODE_AC_BOOKMARKS: return MRB_SYM(ac_bookmarks);
	case SDL_SCANCODE_AC_CLOSE: return MRB_SYM(ac_close);
	case SDL_SCANCODE_AC_EXIT: return MRB_SYM(ac_exit);
	case SDL_SCANCODE_AC_FORWARD: return MRB_SYM(ac_forward);
	case SDL_SCANCODE_AC_HOME: return MRB_SYM(ac_home);
	case SDL_SCANCODE_AC_NEW: return MRB_SYM(ac_new);
	case SDL_SCANCODE_AC_OPEN: return MRB_SYM(ac_open);
	case SDL_SCANCODE_AC_PRINT: return MRB_SYM(ac_print);
	case SDL_SCANCODE_AC_PROPERTIES: return MRB_SYM(ac_properties);
	case SDL_SCANCODE_AC_REFRESH: return MRB_SYM(ac_refresh);
	case SDL_SCANCODE_AC_SAVE: return MRB_SYM(ac_save);
	case SDL_SCANCODE_AC_SEARCH: return MRB_SYM(ac_search);
	case SDL_SCANCODE_AC_STOP: return MRB_SYM(ac_stop);
	case SDL_SCANCODE_AGAIN: return MRB_SYM(again);
	case SDL_SCANCODE_ALTERASE: return MRB_SYM(alterase);
	case SDL_SCANCODE_APOSTROPHE: return MRB_SYM(apostrophe);
	case SDL_SCANCODE_APPLICATION: return MRB_SYM(application);
	case SDL_SCANCODE_B: return MRB_SYM(b);
	case SDL_SCANCODE_BACKSLASH: return MRB_SYM(backslash);
	case SDL_SCANCODE_BACKSPACE: return MRB_SYM(backspace);
	case SDL_SCANCODE_C: return MRB_SYM(c);
	case SDL_SCANCODE_CANCEL: return MRB_SYM(cancel);
	case SDL_SCANCODE_CAPSLOCK: return MRB_SYM(capslock);
	case SDL_SCANCODE_CHANNEL_DECREMENT: return MRB_SYM(channel_decrement);
	case SDL_SCANCODE_CHANNEL_INCREMENT: return MRB_SYM(channel_increment);
	case SDL_SCANCODE_CLEAR: return MRB_SYM(clear);
	case SDL_SCANCODE_CLEARAGAIN: return MRB_SYM(clearagain);
	case SDL_SCANCODE_COMMA: return MRB_SYM(comma);
	case SDL_SCANCODE_COPY: return MRB_SYM(copy);
	case SDL_SCANCODE_CRSEL: return MRB_SYM(crsel);
	case SDL_SCANCODE_CURRENCYSUBUNIT: return MRB_SYM(currencysubunit);
	case SDL_SCANCODE_CURRENCYUNIT: return MRB_SYM(currencyunit);
	case SDL_SCANCODE_CUT: return MRB_SYM(cut);
	case SDL_SCANCODE_D: return MRB_SYM(d);
	case SDL_SCANCODE_DECIMALSEPARATOR: return MRB_SYM(decimalseparator);
	case SDL_SCANCODE_DELETE: return MRB_SYM(delete);
	case SDL_SCANCODE_DOWN: return MRB_SYM(down);
	case SDL_SCANCODE_E: return MRB_SYM(e);
	case SDL_SCANCODE_END: return MRB_SYM(end);
	case SDL_SCANCODE_EQUALS: return MRB_SYM(equals);
	case SDL_SCANCODE_ESCAPE: return MRB_SYM(escape);
	case SDL_SCANCODE_EXECUTE: return MRB_SYM(execute);
	case SDL_SCANCODE_EXSEL: return MRB_SYM(exsel);
	case SDL_SCANCODE_F10: return MRB_SYM(f10);
	case SDL_SCANCODE_F11: return MRB_SYM(f11);
	case SDL_SCANCODE_F12: return MRB_SYM(f12);
	case SDL_SCANCODE_F13: return MRB_SYM(f13);
	case SDL_SCANCODE_F14: return MRB_SYM(f14);
	case SDL_SCANCODE_F15: return MRB_SYM(f15);
	case SDL_SCANCODE_F16: return MRB_SYM(f16);
	case SDL_SCANCODE_F17: return MRB_SYM(f17);
	case SDL_SCANCODE_F18: return MRB_SYM(f18);
	case SDL_SCANCODE_F19: return MRB_SYM(f19);
	case SDL_SCANCODE_F1: return MRB_SYM(f1);
	case SDL_SCANCODE_F20: return MRB_SYM(f20);
	case SDL_SCANCODE_F21: return MRB_SYM(f21);
	case SDL_SCANCODE_F22: return MRB_SYM(f22);
	case SDL_SCANCODE_F23: return MRB_SYM(f23);
	case SDL_SCANCODE_F24: return MRB_SYM(f24);
	case SDL_SCANCODE_F2: return MRB_SYM(f2);
	case SDL_SCANCODE_F3: return MRB_SYM(f3);
	case SDL_SCANCODE_F4: return MRB_SYM(f4);
	case SDL_SCANCODE_F5: return MRB_SYM(f5);
	case SDL_SCANCODE_F6: return MRB_SYM(f6);
	case SDL_SCANCODE_F7: return MRB_SYM(f7);
	case SDL_SCANCODE_F8: return MRB_SYM(f8);
	case SDL_SCANCODE_F9: return MRB_SYM(f9);
	case SDL_SCANCODE_F: return MRB_SYM(f);
	case SDL_SCANCODE_FIND: return MRB_SYM(find);
	case SDL_SCANCODE_G: return MRB_SYM(g);
	case SDL_SCANCODE_GRAVE: return MRB_SYM(grave);
	case SDL_SCANCODE_H: return MRB_SYM(h);
	case SDL_SCANCODE_HELP: return MRB_SYM(help);
	case SDL_SCANCODE_HOME: return MRB_SYM(home);
	case SDL_SCANCODE_I: return MRB_SYM(i);
	case SDL_SCANCODE_INSERT: return MRB_SYM(insert);
	case SDL_SCANCODE_J: return MRB_SYM(j);
	case SDL_SCANCODE_K: return MRB_SYM(k);
	case SDL_SCANCODE_KP_000: return MRB_SYM(kp_000);
	case SDL_SCANCODE_KP_00: return MRB_SYM(kp_00);
	case SDL_SCANCODE_KP_0: return MRB_SYM(kp_0);
	case SDL_SCANCODE_KP_1: return MRB_SYM(kp_1);
	case SDL_SCANCODE_KP_2: return MRB_SYM(kp_2);
	case SDL_SCANCODE_KP_3: return MRB_SYM(kp_3);
	case SDL_SCANCODE_KP_4: return MRB_SYM(kp_4);
	case SDL_SCANCODE_KP_5: return MRB_SYM(kp_5);
	case SDL_SCANCODE_KP_6: return MRB_SYM(kp_6);
	case SDL_SCANCODE_KP_7: return MRB_SYM(kp_7);
	case SDL_SCANCODE_KP_8: return MRB_SYM(kp_8);
	case SDL_SCANCODE_KP_9: return MRB_SYM(kp_9);
	case SDL_SCANCODE_KP_A: return MRB_SYM(kp_a);
	case SDL_SCANCODE_KP_AMPERSAND: return MRB_SYM(kp_ampersand);
	case SDL_SCANCODE_KP_AT: return MRB_SYM(kp_at);
	case SDL_SCANCODE_KP_B: return MRB_SYM(kp_b);
	case SDL_SCANCODE_KP_BACKSPACE: return MRB_SYM(kp_backspace);
	case SDL_SCANCODE_KP_BINARY: return MRB_SYM(kp_binary);
	case SDL_SCANCODE_KP_C: return MRB_SYM(kp_c);
	case SDL_SCANCODE_KP_CLEAR: return MRB_SYM(kp_clear);
	case SDL_SCANCODE_KP_CLEARENTRY: return MRB_SYM(kp_clearentry);
	case SDL_SCANCODE_KP_COLON: return MRB_SYM(kp_colon);
	case SDL_SCANCODE_KP_COMMA: return MRB_SYM(kp_comma);
	case SDL_SCANCODE_KP_D: return MRB_SYM(kp_d);
	case SDL_SCANCODE_KP_DBLAMPERSAND: return MRB_SYM(kp_dblampersand);
	case SDL_SCANCODE_KP_DBLVERTICALBAR: return MRB_SYM(kp_dblverticalbar);
	case SDL_SCANCODE_KP_DECIMAL: return MRB_SYM(kp_decimal);
	case SDL_SCANCODE_KP_DIVIDE: return MRB_SYM(kp_divide);
	case SDL_SCANCODE_KP_E: return MRB_SYM(kp_e);
	case SDL_SCANCODE_KP_ENTER: return MRB_SYM(kp_enter);
	case SDL_SCANCODE_KP_EQUALS: return MRB_SYM(kp_equals);
	case SDL_SCANCODE_KP_EQUALSAS400: return MRB_SYM(kp_equalsas400);
	case SDL_SCANCODE_INTERNATIONAL3: return MRB_SYM(yen);
	case SDL_SCANCODE_LANG1: return MRB_SYM(hangul_english);
	case SDL_SCANCODE_LANG2: return MRB_SYM(hanja);
	case SDL_SCANCODE_LANG3: return MRB_SYM(katakana);
	case SDL_SCANCODE_LANG4: return MRB_SYM(hiragana);
	case SDL_SCANCODE_LANG5: return MRB_SYM(zenkaku_hankaku);
	case SDL_SCANCODE_KP_EXCLAM: return MRB_SYM(kp_exclam);
	case SDL_SCANCODE_KP_F: return MRB_SYM(kp_f);
	case SDL_SCANCODE_KP_GREATER: return MRB_SYM(kp_greater);
	case SDL_SCANCODE_KP_HASH: return MRB_SYM(kp_hash);
	case SDL_SCANCODE_KP_HEXADECIMAL: return MRB_SYM(kp_hexadecimal);
	case SDL_SCANCODE_KP_LEFTBRACE: return MRB_SYM(kp_leftbrace);
	case SDL_SCANCODE_KP_LEFTPAREN: return MRB_SYM(kp_leftparen);
	case SDL_SCANCODE_KP_LESS: return MRB_SYM(kp_less);
	case SDL_SCANCODE_KP_MEMADD: return MRB_SYM(kp_memadd);
	case SDL_SCANCODE_KP_MEMCLEAR: return MRB_SYM(kp_memclear);
	case SDL_SCANCODE_KP_MEMDIVIDE: return MRB_SYM(kp_memdivide);
	case SDL_SCANCODE_KP_MEMMULTIPLY: return MRB_SYM(kp_memmultiply);
	case SDL_SCANCODE_KP_MEMRECALL: return MRB_SYM(kp_memrecall);
	case SDL_SCANCODE_KP_MEMSTORE: return MRB_SYM(kp_memstore);
	case SDL_SCANCODE_KP_MEMSUBTRACT: return MRB_SYM(kp_memsubtract);
	case SDL_SCANCODE_KP_MINUS: return MRB_SYM(kp_minus);
	case SDL_SCANCODE_KP_MULTIPLY: return MRB_SYM(kp_multiply);
	case SDL_SCANCODE_KP_OCTAL: return MRB_SYM(kp_octal);
	case SDL_SCANCODE_KP_PERCENT: return MRB_SYM(kp_percent);
	case SDL_SCANCODE_KP_PERIOD: return MRB_SYM(kp_period);
	case SDL_SCANCODE_KP_PLUS: return MRB_SYM(kp_plus);
	case SDL_SCANCODE_KP_PLUSMINUS: return MRB_SYM(kp_plusminus);
	case SDL_SCANCODE_KP_POWER: return MRB_SYM(kp_power);
	case SDL_SCANCODE_KP_RIGHTBRACE: return MRB_SYM(kp_rightbrace);
	case SDL_SCANCODE_KP_RIGHTPAREN: return MRB_SYM(kp_rightparen);
	case SDL_SCANCODE_KP_SPACE: return MRB_SYM(kp_space);
	case SDL_SCANCODE_KP_TAB: return MRB_SYM(kp_tab);
	case SDL_SCANCODE_KP_VERTICALBAR: return MRB_SYM(kp_verticalbar);
	case SDL_SCANCODE_KP_XOR: return MRB_SYM(kp_xor);
	case SDL_SCANCODE_L: return MRB_SYM(l);
	case SDL_SCANCODE_LALT: return MRB_SYM(lalt);
	case SDL_SCANCODE_LCTRL: return MRB_SYM(lctrl);
	case SDL_SCANCODE_LEFT: return MRB_SYM(left);
	case SDL_SCANCODE_LEFTBRACKET: return MRB_SYM(leftbracket);
	case SDL_SCANCODE_LGUI: return MRB_SYM(lgui);
	case SDL_SCANCODE_LSHIFT: return MRB_SYM(lshift);
	case SDL_SCANCODE_M: return MRB_SYM(m);
	case SDL_SCANCODE_MEDIA_EJECT: return MRB_SYM(media_eject);
	case SDL_SCANCODE_MEDIA_FAST_FORWARD:
		return MRB_SYM(media_fast_forward);
	case SDL_SCANCODE_MEDIA_NEXT_TRACK: return MRB_SYM(media_next_track);
	case SDL_SCANCODE_MEDIA_PAUSE: return MRB_SYM(media_pause);
	case SDL_SCANCODE_MEDIA_PLAY: return MRB_SYM(media_play);
	case SDL_SCANCODE_MEDIA_PLAY_PAUSE: return MRB_SYM(media_play_pause);
	case SDL_SCANCODE_MEDIA_PREVIOUS_TRACK:
		return MRB_SYM(media_previous_track);
	case SDL_SCANCODE_MEDIA_RECORD: return MRB_SYM(media_record);
	case SDL_SCANCODE_MEDIA_REWIND: return MRB_SYM(media_rewind);
	case SDL_SCANCODE_MEDIA_SELECT: return MRB_SYM(media_select);
	case SDL_SCANCODE_MEDIA_STOP: return MRB_SYM(media_stop);
	case SDL_SCANCODE_MENU: return MRB_SYM(menu);
	case SDL_SCANCODE_MINUS: return MRB_SYM(minus);
	case SDL_SCANCODE_MODE: return MRB_SYM(mode);
	case SDL_SCANCODE_MUTE: return MRB_SYM(mute);
	case SDL_SCANCODE_N: return MRB_SYM(n);
	case SDL_SCANCODE_NONUSBACKSLASH: return MRB_SYM(nonusbackslash);
	case SDL_SCANCODE_NONUSHASH: return MRB_SYM(nonushash);
	case SDL_SCANCODE_NUMLOCKCLEAR: return MRB_SYM(numlockclear);
	case SDL_SCANCODE_O: return MRB_SYM(o);
	case SDL_SCANCODE_OPER: return MRB_SYM(oper);
	case SDL_SCANCODE_OUT: return MRB_SYM(out);
	case SDL_SCANCODE_P: return MRB_SYM(p);
	case SDL_SCANCODE_PAGEDOWN: return MRB_SYM(pagedown);
	case SDL_SCANCODE_PAGEUP: return MRB_SYM(pageup);
	case SDL_SCANCODE_PASTE: return MRB_SYM(paste);
	case SDL_SCANCODE_PAUSE: return MRB_SYM(pause);
	case SDL_SCANCODE_PERIOD: return MRB_SYM(period);
	case SDL_SCANCODE_POWER: return MRB_SYM(power);
	case SDL_SCANCODE_PRINTSCREEN: return MRB_SYM(printscreen);
	case SDL_SCANCODE_PRIOR: return MRB_SYM(prior);
	case SDL_SCANCODE_Q: return MRB_SYM(q);
	case SDL_SCANCODE_R: return MRB_SYM(r);
	case SDL_SCANCODE_RALT: return MRB_SYM(ralt);
	case SDL_SCANCODE_RCTRL: return MRB_SYM(rctrl);
	case SDL_SCANCODE_RETURN2: return MRB_SYM(return2);
	case SDL_SCANCODE_RETURN: return MRB_SYM(return);
	case SDL_SCANCODE_RGUI: return MRB_SYM(rgui);
	case SDL_SCANCODE_RIGHT: return MRB_SYM(right);
	case SDL_SCANCODE_RIGHTBRACKET: return MRB_SYM(rightbracket);
	case SDL_SCANCODE_RSHIFT: return MRB_SYM(rshift);
	case SDL_SCANCODE_S: return MRB_SYM(s);
	case SDL_SCANCODE_SCROLLLOCK: return MRB_SYM(scrolllock);
	case SDL_SCANCODE_SELECT: return MRB_SYM(select);
	case SDL_SCANCODE_SEMICOLON: return MRB_SYM(semicolon);
	case SDL_SCANCODE_SEPARATOR: return MRB_SYM(separator);
	case SDL_SCANCODE_SLASH: return MRB_SYM(slash);
	case SDL_SCANCODE_SLEEP: return MRB_SYM(sleep);
	case SDL_SCANCODE_SPACE: return MRB_SYM(space);
	case SDL_SCANCODE_STOP: return MRB_SYM(stop);
	case SDL_SCANCODE_SYSREQ: return MRB_SYM(sysreq);
	case SDL_SCANCODE_T: return MRB_SYM(t);
	case SDL_SCANCODE_TAB: return MRB_SYM(tab);
	case SDL_SCANCODE_THOUSANDSSEPARATOR:
		return MRB_SYM(thousandsseparator);
	case SDL_SCANCODE_U: return MRB_SYM(u);
	case SDL_SCANCODE_UNDO: return MRB_SYM(undo);
	case SDL_SCANCODE_UP: return MRB_SYM(up);
	case SDL_SCANCODE_V: return MRB_SYM(v);
	case SDL_SCANCODE_VOLUMEDOWN: return MRB_SYM(volumedown);
	case SDL_SCANCODE_VOLUMEUP: return MRB_SYM(volumeup);
	case SDL_SCANCODE_W: return MRB_SYM(w);
	case SDL_SCANCODE_WAKE: return MRB_SYM(wake);
	case SDL_SCANCODE_X: return MRB_SYM(x);
	case SDL_SCANCODE_Y: return MRB_SYM(y);
	case SDL_SCANCODE_Z: return MRB_SYM(z);
	default: return MRB_SYM(unknown);
	}
}

static mrb_sym
sdl_keycode_sym(mrb_state *mrb, SDL_Keycode code)
{
	switch (code) {
	case SDLK_RETURN: return MRB_SYM(return);
	case SDLK_ESCAPE: return MRB_SYM(escape);
	case SDLK_BACKSPACE: return MRB_SYM(backspace);
	case SDLK_TAB: return MRB_SYM(tab);
	case SDLK_SPACE: return MRB_SYM(space);
	case SDLK_EXCLAIM: return MRB_SYM(exclaim);
	case SDLK_DBLAPOSTROPHE: return MRB_SYM(dblapostrophe);
	case SDLK_HASH: return MRB_SYM(hash);
	case SDLK_DOLLAR: return MRB_SYM(dollar);
	case SDLK_PERCENT: return MRB_SYM(percent);
	case SDLK_AMPERSAND: return MRB_SYM(ampersand);
	case SDLK_APOSTROPHE: return MRB_SYM(apostrophe);
	case SDLK_LEFTPAREN: return MRB_SYM(leftparen);
	case SDLK_RIGHTPAREN: return MRB_SYM(rightparen);
	case SDLK_ASTERISK: return MRB_SYM(asterisk);
	case SDLK_PLUS: return MRB_SYM(plus);
	case SDLK_COMMA: return MRB_SYM(comma);
	case SDLK_MINUS: return MRB_SYM(minus);
	case SDLK_PERIOD: return MRB_SYM(period);
	case SDLK_SLASH: return MRB_SYM(slash);
	case SDLK_0: return mrb_intern_lit(mrb, "0");
	case SDLK_1: return mrb_intern_lit(mrb, "1");
	case SDLK_2: return mrb_intern_lit(mrb, "2");
	case SDLK_3: return mrb_intern_lit(mrb, "3");
	case SDLK_4: return mrb_intern_lit(mrb, "4");
	case SDLK_5: return mrb_intern_lit(mrb, "5");
	case SDLK_6: return mrb_intern_lit(mrb, "6");
	case SDLK_7: return mrb_intern_lit(mrb, "7");
	case SDLK_8: return mrb_intern_lit(mrb, "8");
	case SDLK_9: return mrb_intern_lit(mrb, "9");
	case SDLK_COLON: return MRB_SYM(colon);
	case SDLK_SEMICOLON: return MRB_SYM(semicolon);
	case SDLK_LESS: return MRB_SYM(less);
	case SDLK_EQUALS: return MRB_SYM(equals);
	case SDLK_GREATER: return MRB_SYM(greater);
	case SDLK_QUESTION: return MRB_SYM(question);
	case SDLK_AT: return MRB_SYM(at);
	case SDLK_LEFTBRACKET: return MRB_SYM(leftbracket);
	case SDLK_BACKSLASH: return MRB_SYM(backslash);
	case SDLK_RIGHTBRACKET: return MRB_SYM(rightbracket);
	case SDLK_CARET: return MRB_SYM(caret);
	case SDLK_UNDERSCORE: return MRB_SYM(underscore);
	case SDLK_GRAVE: return MRB_SYM(grave);
	case SDLK_A: return MRB_SYM(a);
	case SDLK_B: return MRB_SYM(b);
	case SDLK_C: return MRB_SYM(c);
	case SDLK_D: return MRB_SYM(d);
	case SDLK_E: return MRB_SYM(e);
	case SDLK_F: return MRB_SYM(f);
	case SDLK_G: return MRB_SYM(g);
	case SDLK_H: return MRB_SYM(h);
	case SDLK_I: return MRB_SYM(i);
	case SDLK_J: return MRB_SYM(j);
	case SDLK_K: return MRB_SYM(k);
	case SDLK_L: return MRB_SYM(l);
	case SDLK_M: return MRB_SYM(m);
	case SDLK_N: return MRB_SYM(n);
	case SDLK_O: return MRB_SYM(o);
	case SDLK_P: return MRB_SYM(p);
	case SDLK_Q: return MRB_SYM(q);
	case SDLK_R: return MRB_SYM(r);
	case SDLK_S: return MRB_SYM(s);
	case SDLK_T: return MRB_SYM(t);
	case SDLK_U: return MRB_SYM(u);
	case SDLK_V: return MRB_SYM(v);
	case SDLK_W: return MRB_SYM(w);
	case SDLK_X: return MRB_SYM(x);
	case SDLK_Y: return MRB_SYM(y);
	case SDLK_Z: return MRB_SYM(z);
	case SDLK_LEFTBRACE: return MRB_SYM(leftbrace);
	case SDLK_PIPE: return MRB_SYM(pipe);
	case SDLK_RIGHTBRACE: return MRB_SYM(rightbrace);
	case SDLK_TILDE: return MRB_SYM(tilde);
	case SDLK_DELETE: return MRB_SYM(delete);
	case SDLK_PLUSMINUS: return MRB_SYM(plusminus);
	case SDLK_CAPSLOCK: return MRB_SYM(capslock);
	case SDLK_F1: return MRB_SYM(f1);
	case SDLK_F2: return MRB_SYM(f2);
	case SDLK_F3: return MRB_SYM(f3);
	case SDLK_F4: return MRB_SYM(f4);
	case SDLK_F5: return MRB_SYM(f5);
	case SDLK_F6: return MRB_SYM(f6);
	case SDLK_F7: return MRB_SYM(f7);
	case SDLK_F8: return MRB_SYM(f8);
	case SDLK_F9: return MRB_SYM(f9);
	case SDLK_F10: return MRB_SYM(f10);
	case SDLK_F11: return MRB_SYM(f11);
	case SDLK_F12: return MRB_SYM(f12);
	case SDLK_PRINTSCREEN: return MRB_SYM(printscreen);
	case SDLK_SCROLLLOCK: return MRB_SYM(scrolllock);
	case SDLK_PAUSE: return MRB_SYM(pause);
	case SDLK_INSERT: return MRB_SYM(insert);
	case SDLK_HOME: return MRB_SYM(home);
	case SDLK_PAGEUP: return MRB_SYM(pageup);
	case SDLK_END: return MRB_SYM(end);
	case SDLK_PAGEDOWN: return MRB_SYM(pagedown);
	case SDLK_RIGHT: return MRB_SYM(right);
	case SDLK_LEFT: return MRB_SYM(left);
	case SDLK_DOWN: return MRB_SYM(down);
	case SDLK_UP: return MRB_SYM(up);
	case SDLK_NUMLOCKCLEAR: return MRB_SYM(numlockclear);
	case SDLK_KP_DIVIDE: return MRB_SYM(kp_divide);
	case SDLK_KP_MULTIPLY: return MRB_SYM(kp_multiply);
	case SDLK_KP_MINUS: return MRB_SYM(kp_minus);
	case SDLK_KP_PLUS: return MRB_SYM(kp_plus);
	case SDLK_KP_ENTER: return MRB_SYM(kp_enter);
	case SDLK_KP_1: return MRB_SYM(kp_1);
	case SDLK_KP_2: return MRB_SYM(kp_2);
	case SDLK_KP_3: return MRB_SYM(kp_3);
	case SDLK_KP_4: return MRB_SYM(kp_4);
	case SDLK_KP_5: return MRB_SYM(kp_5);
	case SDLK_KP_6: return MRB_SYM(kp_6);
	case SDLK_KP_7: return MRB_SYM(kp_7);
	case SDLK_KP_8: return MRB_SYM(kp_8);
	case SDLK_KP_9: return MRB_SYM(kp_9);
	case SDLK_KP_0: return MRB_SYM(kp_0);
	case SDLK_KP_PERIOD: return MRB_SYM(kp_period);
	case SDLK_APPLICATION: return MRB_SYM(application);
	case SDLK_POWER: return MRB_SYM(power);
	case SDLK_KP_EQUALS: return MRB_SYM(kp_equals);
	case SDLK_F13: return MRB_SYM(f13);
	case SDLK_F14: return MRB_SYM(f14);
	case SDLK_F15: return MRB_SYM(f15);
	case SDLK_F16: return MRB_SYM(f16);
	case SDLK_F17: return MRB_SYM(f17);
	case SDLK_F18: return MRB_SYM(f18);
	case SDLK_F19: return MRB_SYM(f19);
	case SDLK_F20: return MRB_SYM(f20);
	case SDLK_F21: return MRB_SYM(f21);
	case SDLK_F22: return MRB_SYM(f22);
	case SDLK_F23: return MRB_SYM(f23);
	case SDLK_F24: return MRB_SYM(f24);
	case SDLK_EXECUTE: return MRB_SYM(execute);
	case SDLK_HELP: return MRB_SYM(help);
	case SDLK_MENU: return MRB_SYM(menu);
	case SDLK_SELECT: return MRB_SYM(select);
	case SDLK_STOP: return MRB_SYM(stop);
	case SDLK_AGAIN: return MRB_SYM(again);
	case SDLK_UNDO: return MRB_SYM(undo);
	case SDLK_CUT: return MRB_SYM(cut);
	case SDLK_COPY: return MRB_SYM(copy);
	case SDLK_PASTE: return MRB_SYM(paste);
	case SDLK_FIND: return MRB_SYM(find);
	case SDLK_MUTE: return MRB_SYM(mute);
	case SDLK_VOLUMEUP: return MRB_SYM(volumeup);
	case SDLK_VOLUMEDOWN: return MRB_SYM(volumedown);
	case SDLK_KP_COMMA: return MRB_SYM(kp_comma);
	case SDLK_KP_EQUALSAS400: return MRB_SYM(kp_equalsas400);
	case SDLK_ALTERASE: return MRB_SYM(alterase);
	case SDLK_SYSREQ: return MRB_SYM(sysreq);
	case SDLK_CANCEL: return MRB_SYM(cancel);
	case SDLK_CLEAR: return MRB_SYM(clear);
	case SDLK_PRIOR: return MRB_SYM(prior);
	case SDLK_RETURN2: return MRB_SYM(return2);
	case SDLK_SEPARATOR: return MRB_SYM(separator);
	case SDLK_OUT: return MRB_SYM(out);
	case SDLK_OPER: return MRB_SYM(oper);
	case SDLK_CLEARAGAIN: return MRB_SYM(clearagain);
	case SDLK_CRSEL: return MRB_SYM(crsel);
	case SDLK_EXSEL: return MRB_SYM(exsel);
	case SDLK_KP_00: return MRB_SYM(kp_00);
	case SDLK_KP_000: return MRB_SYM(kp_000);
	case SDLK_THOUSANDSSEPARATOR: return MRB_SYM(thousandsseparator);
	case SDLK_DECIMALSEPARATOR: return MRB_SYM(decimalseparator);
	case SDLK_CURRENCYUNIT: return MRB_SYM(currencyunit);
	case SDLK_CURRENCYSUBUNIT: return MRB_SYM(currencysubunit);
	case SDLK_KP_LEFTPAREN: return MRB_SYM(kp_leftparen);
	case SDLK_KP_RIGHTPAREN: return MRB_SYM(kp_rightparen);
	case SDLK_KP_LEFTBRACE: return MRB_SYM(kp_leftbrace);
	case SDLK_KP_RIGHTBRACE: return MRB_SYM(kp_rightbrace);
	case SDLK_KP_TAB: return MRB_SYM(kp_tab);
	case SDLK_KP_BACKSPACE: return MRB_SYM(kp_backspace);
	case SDLK_KP_A: return MRB_SYM(kp_a);
	case SDLK_KP_B: return MRB_SYM(kp_b);
	case SDLK_KP_C: return MRB_SYM(kp_c);
	case SDLK_KP_D: return MRB_SYM(kp_d);
	case SDLK_KP_E: return MRB_SYM(kp_e);
	case SDLK_KP_F: return MRB_SYM(kp_f);
	case SDLK_KP_XOR: return MRB_SYM(kp_xor);
	case SDLK_KP_POWER: return MRB_SYM(kp_power);
	case SDLK_KP_PERCENT: return MRB_SYM(kp_percent);
	case SDLK_KP_LESS: return MRB_SYM(kp_less);
	case SDLK_KP_GREATER: return MRB_SYM(kp_greater);
	case SDLK_KP_AMPERSAND: return MRB_SYM(kp_ampersand);
	case SDLK_KP_DBLAMPERSAND: return MRB_SYM(kp_dblampersand);
	case SDLK_KP_VERTICALBAR: return MRB_SYM(kp_verticalbar);
	case SDLK_KP_DBLVERTICALBAR: return MRB_SYM(kp_dblverticalbar);
	case SDLK_KP_COLON: return MRB_SYM(kp_colon);
	case SDLK_KP_HASH: return MRB_SYM(kp_hash);
	case SDLK_KP_SPACE: return MRB_SYM(kp_space);
	case SDLK_KP_AT: return MRB_SYM(kp_at);
	case SDLK_KP_EXCLAM: return MRB_SYM(kp_exclam);
	case SDLK_KP_MEMSTORE: return MRB_SYM(kp_memstore);
	case SDLK_KP_MEMRECALL: return MRB_SYM(kp_memrecall);
	case SDLK_KP_MEMCLEAR: return MRB_SYM(kp_memclear);
	case SDLK_KP_MEMADD: return MRB_SYM(kp_memadd);
	case SDLK_KP_MEMSUBTRACT: return MRB_SYM(kp_memsubtract);
	case SDLK_KP_MEMMULTIPLY: return MRB_SYM(kp_memmultiply);
	case SDLK_KP_MEMDIVIDE: return MRB_SYM(kp_memdivide);
	case SDLK_KP_PLUSMINUS: return MRB_SYM(kp_plusminus);
	case SDLK_KP_CLEAR: return MRB_SYM(kp_clear);
	case SDLK_KP_CLEARENTRY: return MRB_SYM(kp_clearentry);
	case SDLK_KP_BINARY: return MRB_SYM(kp_binary);
	case SDLK_KP_OCTAL: return MRB_SYM(kp_octal);
	case SDLK_KP_DECIMAL: return MRB_SYM(kp_decimal);
	case SDLK_KP_HEXADECIMAL: return MRB_SYM(kp_hexadecimal);
	case SDLK_LCTRL: return MRB_SYM(lctrl);
	case SDLK_LSHIFT: return MRB_SYM(lshift);
	case SDLK_LALT: return MRB_SYM(lalt);
	case SDLK_LGUI: return MRB_SYM(lgui);
	case SDLK_RCTRL: return MRB_SYM(rctrl);
	case SDLK_RSHIFT: return MRB_SYM(rshift);
	case SDLK_RALT: return MRB_SYM(ralt);
	case SDLK_RGUI: return MRB_SYM(rgui);
	case SDLK_MODE: return MRB_SYM(mode);
	case SDLK_SLEEP: return MRB_SYM(sleep);
	case SDLK_WAKE: return MRB_SYM(wake);
	case SDLK_CHANNEL_INCREMENT: return MRB_SYM(channel_increment);
	case SDLK_CHANNEL_DECREMENT: return MRB_SYM(channel_decrement);
	case SDLK_MEDIA_PLAY: return MRB_SYM(media_play);
	case SDLK_MEDIA_PAUSE: return MRB_SYM(media_pause);
	case SDLK_MEDIA_RECORD: return MRB_SYM(media_record);
	case SDLK_MEDIA_FAST_FORWARD: return MRB_SYM(media_fast_forward);
	case SDLK_MEDIA_REWIND: return MRB_SYM(media_rewind);
	case SDLK_MEDIA_NEXT_TRACK: return MRB_SYM(media_next_track);
	case SDLK_MEDIA_PREVIOUS_TRACK: return MRB_SYM(media_previous_track);
	case SDLK_MEDIA_STOP: return MRB_SYM(media_stop);
	case SDLK_MEDIA_EJECT: return MRB_SYM(media_eject);
	case SDLK_MEDIA_PLAY_PAUSE: return MRB_SYM(media_play_pause);
	case SDLK_MEDIA_SELECT: return MRB_SYM(media_select);
	case SDLK_AC_NEW: return MRB_SYM(ac_new);
	case SDLK_AC_OPEN: return MRB_SYM(ac_open);
	case SDLK_AC_CLOSE: return MRB_SYM(ac_close);
	case SDLK_AC_EXIT: return MRB_SYM(ac_exit);
	case SDLK_AC_SAVE: return MRB_SYM(ac_save);
	case SDLK_AC_PRINT: return MRB_SYM(ac_print);
	case SDLK_AC_PROPERTIES: return MRB_SYM(ac_properties);
	case SDLK_AC_SEARCH: return MRB_SYM(ac_search);
	case SDLK_AC_HOME: return MRB_SYM(ac_home);
	case SDLK_AC_BACK: return MRB_SYM(ac_back);
	case SDLK_AC_FORWARD: return MRB_SYM(ac_forward);
	case SDLK_AC_STOP: return MRB_SYM(ac_stop);
	case SDLK_AC_REFRESH: return MRB_SYM(ac_refresh);
	case SDLK_AC_BOOKMARKS: return MRB_SYM(ac_bookmarks);
	case SDLK_SOFTLEFT: return MRB_SYM(softleft);
	case SDLK_SOFTRIGHT: return MRB_SYM(softright);
	case SDLK_CALL: return MRB_SYM(call);
	case SDLK_ENDCALL: return MRB_SYM(endcall);
	case SDLK_LEFT_TAB: return MRB_SYM(left_tab);
	case SDLK_LEVEL5_SHIFT: return MRB_SYM(level5_shift);
	case SDLK_MULTI_KEY_COMPOSE: return MRB_SYM(multi_key_compose);
	case SDLK_LMETA: return MRB_SYM(lmeta);
	case SDLK_RMETA: return MRB_SYM(rmeta);
	case SDLK_LHYPER: return MRB_SYM(lhyper);
	case SDLK_RHYPER: return MRB_SYM(rhyper);
	default: return MRB_SYM(unknown);
	}
}

static mrb_sym
sdl_kmod_sym(const SDL_Keymod code)
{
	switch (code) {
	case SDL_KMOD_LSHIFT: return MRB_SYM(lshift);
	case SDL_KMOD_RSHIFT: return MRB_SYM(rshift);
	case SDL_KMOD_LEVEL5: return MRB_SYM(level5);
	case SDL_KMOD_LCTRL: return MRB_SYM(lctrl);
	case SDL_KMOD_RCTRL: return MRB_SYM(rctrl);
	case SDL_KMOD_LALT: return MRB_SYM(lalt);
	case SDL_KMOD_RALT: return MRB_SYM(ralt);
	case SDL_KMOD_LGUI: return MRB_SYM(lgui);
	case SDL_KMOD_RGUI: return MRB_SYM(rgui);
	case SDL_KMOD_NUM: return MRB_SYM(num);
	case SDL_KMOD_CAPS: return MRB_SYM(caps);
	case SDL_KMOD_MODE: return MRB_SYM(mode);
	case SDL_KMOD_SCROLL: return MRB_SYM(scroll);
	default: return MRB_SYM(none);
	}
}

static mrb_sym
sdl_jhat_sym(const uint8_t hat)
{
	switch (hat) {
	case SDL_HAT_LEFTUP: return MRB_SYM(leftup);
	case SDL_HAT_UP: return MRB_SYM(up);
	case SDL_HAT_RIGHTUP: return MRB_SYM(rightup);
	case SDL_HAT_LEFT: return MRB_SYM(left);
	case SDL_HAT_RIGHT: return MRB_SYM(right);
	case SDL_HAT_LEFTDOWN: return MRB_SYM(leftdown);
	case SDL_HAT_DOWN: return MRB_SYM(down);
	case SDL_HAT_RIGHTDOWN: return MRB_SYM(rightdown);
	default: return MRB_SYM(centered);
	}
}

static mrb_sym
sdl_power_sym(const SDL_PowerState level)
{
	switch (level) {
	case SDL_POWERSTATE_ON_BATTERY: return MRB_SYM(on_battery);
	case SDL_POWERSTATE_NO_BATTERY: return MRB_SYM(no_battery);
	case SDL_POWERSTATE_CHARGING: return MRB_SYM(charging);
	case SDL_POWERSTATE_CHARGED: return MRB_SYM(charged);
	default: return MRB_SYM(unknown);
	}
}

static mrb_sym
sdl_gamepad_axis_sym(const SDL_GamepadAxis axis)
{
	switch (axis) {
	case SDL_GAMEPAD_AXIS_LEFTX: return MRB_SYM(leftx);
	case SDL_GAMEPAD_AXIS_LEFTY: return MRB_SYM(lefty);
	case SDL_GAMEPAD_AXIS_RIGHTX: return MRB_SYM(rightx);
	case SDL_GAMEPAD_AXIS_RIGHTY: return MRB_SYM(righty);
	case SDL_GAMEPAD_AXIS_LEFT_TRIGGER: return MRB_SYM(left_trigger);
	case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER: return MRB_SYM(right_trigger);
	default: return MRB_SYM(invalid);
	}
}

static mrb_sym
sdl_gamepad_button_sym(const SDL_GamepadButton button)
{
	switch (button) {
	case SDL_GAMEPAD_BUTTON_SOUTH: return MRB_SYM(south);
	case SDL_GAMEPAD_BUTTON_EAST: return MRB_SYM(east);
	case SDL_GAMEPAD_BUTTON_WEST: return MRB_SYM(west);
	case SDL_GAMEPAD_BUTTON_NORTH: return MRB_SYM(north);
	case SDL_GAMEPAD_BUTTON_BACK: return MRB_SYM(back);
	case SDL_GAMEPAD_BUTTON_GUIDE: return MRB_SYM(guide);
	case SDL_GAMEPAD_BUTTON_START: return MRB_SYM(start);
	case SDL_GAMEPAD_BUTTON_LEFT_STICK: return MRB_SYM(left_stick);
	case SDL_GAMEPAD_BUTTON_RIGHT_STICK: return MRB_SYM(right_stick);
	case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER: return MRB_SYM(left_shoulder);
	case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER: return MRB_SYM(right_shoulder);
	case SDL_GAMEPAD_BUTTON_DPAD_UP: return MRB_SYM(dpad_up);
	case SDL_GAMEPAD_BUTTON_DPAD_DOWN: return MRB_SYM(dpad_down);
	case SDL_GAMEPAD_BUTTON_DPAD_LEFT: return MRB_SYM(dpad_left);
	case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: return MRB_SYM(dpad_right);
	case SDL_GAMEPAD_BUTTON_MISC1: return MRB_SYM(misc1);
	case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1: return MRB_SYM(right_paddle1);
	case SDL_GAMEPAD_BUTTON_LEFT_PADDLE1: return MRB_SYM(left_paddle1);
	case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2: return MRB_SYM(right_paddle2);
	case SDL_GAMEPAD_BUTTON_LEFT_PADDLE2: return MRB_SYM(left_paddle2);
	case SDL_GAMEPAD_BUTTON_TOUCHPAD: return MRB_SYM(touchpad);
	case SDL_GAMEPAD_BUTTON_MISC2: return MRB_SYM(misc2);
	case SDL_GAMEPAD_BUTTON_MISC3: return MRB_SYM(misc3);
	case SDL_GAMEPAD_BUTTON_MISC4: return MRB_SYM(misc4);
	case SDL_GAMEPAD_BUTTON_MISC5: return MRB_SYM(misc5);
	case SDL_GAMEPAD_BUTTON_MISC6: return MRB_SYM(misc6);
	default: return MRB_SYM(invalid);
	}
}

static mrb_sym
sdl_pen_input_sym(const SDL_PenInputFlags flags)
{
	switch (flags) {
	case SDL_PEN_INPUT_DOWN: return MRB_SYM(down);
	case SDL_PEN_INPUT_BUTTON_1: return MRB_SYM(button_1);
	case SDL_PEN_INPUT_BUTTON_2: return MRB_SYM(button_2);
	case SDL_PEN_INPUT_BUTTON_3: return MRB_SYM(button_3);
	case SDL_PEN_INPUT_BUTTON_4: return MRB_SYM(button_4);
	case SDL_PEN_INPUT_BUTTON_5: return MRB_SYM(button_5);
	case SDL_PEN_INPUT_ERASER_TIP: return MRB_SYM(eraser_tip);
	default: return MRB_SYM(invalid);
	}
}

static mrb_sym
sdl_pen_axis_sym(const SDL_PenAxis axis)
{
	switch (axis) {
	case SDL_PEN_AXIS_PRESSURE: return MRB_SYM(pressure);
	case SDL_PEN_AXIS_XTILT: return MRB_SYM(xtilt);
	case SDL_PEN_AXIS_YTILT: return MRB_SYM(ytilt);
	case SDL_PEN_AXIS_DISTANCE: return MRB_SYM(distance);
	case SDL_PEN_AXIS_ROTATION: return MRB_SYM(rotation);
	case SDL_PEN_AXIS_SLIDER: return MRB_SYM(slider);
	case SDL_PEN_AXIS_TANGENTIAL_PRESSURE:
		return MRB_SYM(tangential_pressure);
	default: return MRB_SYM(invalid);
	}
}

mrb_value
euler::game::sdl_event_to_mrb(util::Reference<State> state,
    const SDL_Event &event)
{
	auto mrb = state->mrb();
	mrb_value value = mrb_nil_value();
	auto mod = state->module().game;
	auto arena_index = mrb_gc_arena_save(mrb);
	const auto type = sdl_event_sym(mrb, event);
	const auto type_val = mrb_symbol_value(type);

#define SET_INT_IV(ATTR, FIELD)                                                \
	do {                                                                   \
		const auto sym = MRB_IVSYM(ATTR);                              \
		const auto iv                                                  \
		    = mrb_int_value(mrb, static_cast<mrb_int>(event.FIELD));   \
		mrb_iv_set(mrb, value, sym, iv);                               \
	} while (0)

#define SET_BOOL_IV(ATTR, FIELD)                                               \
	do {                                                                   \
		const auto sym = MRB_IVSYM(ATTR);                              \
		const auto iv                                                  \
		    = mrb_bool_value(static_cast<bool>(event.FIELD));          \
		mrb_iv_set(mrb, value, sym, iv);                               \
	} while (0)

#define SET_CSTRING_IV(ATTR, FIELD)                                            \
	do {                                                                   \
		const auto sym = MRB_IVSYM(ATTR);                              \
		const auto iv = mrb_str_new_cstr(mrb, event.FIELD);            \
		mrb_iv_set(mrb, value, sym, iv);                               \
	} while (0)

#define SET_SYM_IV(ATTR, VALUE)                                                \
	do {                                                                   \
		const auto sym = MRB_IVSYM(ATTR);                              \
		const auto val = (VALUE);                                      \
		const auto sym_val = mrb_symbol_value(val);                    \
		mrb_iv_set(mrb, value, sym, sym_val);                          \
	} while (0)

#define SET_FLOAT_IV(ATTR, FIELD)                                              \
	do {                                                                   \
		const auto sym = MRB_IVSYM(ATTR);                              \
		const auto iv = mrb_float_value(mrb,                           \
		    static_cast<mrb_float>(event.FIELD));                      \
		mrb_iv_set(mrb, value, sym, iv);                               \
	} while (0)

#define COMMON_INIT(SYM)                                                       \
	do {                                                                   \
		value = mrb_obj_new(mrb, mod.SYM##_event, 0, nullptr);         \
		assert(!mrb_nil_p(value));                                     \
		SET_INT_IV(timestamp, common.timestamp);                       \
		mrb_iv_set(mrb, value, MRB_IVSYM(type), type_val);             \
	} while (0)

	switch (static_cast<SDL_EventType>(event.type)) {
	case SDL_EVENT_QUIT: COMMON_INIT(quit); break;
	case SDL_EVENT_DISPLAY_ORIENTATION:
	case SDL_EVENT_DISPLAY_ADDED:
	case SDL_EVENT_DISPLAY_REMOVED:
	case SDL_EVENT_DISPLAY_MOVED:
	case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED:
	case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED: [[fallthrough]];
	case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
		COMMON_INIT(display);
		SET_INT_IV(display_id, display.displayID);
		if (event.type == SDL_EVENT_DISPLAY_ORIENTATION) {
			SET_INT_IV(orientation, display.data1);
		}
		break;
	case SDL_EVENT_WINDOW_SHOWN:
	case SDL_EVENT_WINDOW_HIDDEN:
	case SDL_EVENT_WINDOW_EXPOSED:
	case SDL_EVENT_WINDOW_MOVED:
	case SDL_EVENT_WINDOW_RESIZED:
	case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
	case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
	case SDL_EVENT_WINDOW_MINIMIZED:
	case SDL_EVENT_WINDOW_MAXIMIZED:
	case SDL_EVENT_WINDOW_RESTORED:
	case SDL_EVENT_WINDOW_MOUSE_ENTER:
	case SDL_EVENT_WINDOW_MOUSE_LEAVE:
	case SDL_EVENT_WINDOW_FOCUS_GAINED:
	case SDL_EVENT_WINDOW_FOCUS_LOST:
	case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
	case SDL_EVENT_WINDOW_HIT_TEST:
	case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
	case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
	case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
	case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
	case SDL_EVENT_WINDOW_OCCLUDED:
	case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
	case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
	case SDL_EVENT_WINDOW_DESTROYED: [[fallthrough]];
	case SDL_EVENT_WINDOW_HDR_STATE_CHANGED:
		COMMON_INIT(window);
		SET_INT_IV(window_id, window.windowID);
		switch (event.type) {
		case SDL_EVENT_WINDOW_MOVED:
			SET_INT_IV(x, window.data1);
			SET_INT_IV(y, window.data2);
			break;
		case SDL_EVENT_WINDOW_RESIZED:
		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
			SET_INT_IV(width, window.data1);
			SET_INT_IV(height, window.data2);
			break;
		case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
			SET_INT_IV(display_id, window.data1);
			break;
		default: break;
		}
		break;
	case SDL_EVENT_KEY_DOWN: [[fallthrough]];
	case SDL_EVENT_KEY_UP:
		COMMON_INIT(keyboard);
		SET_BOOL_IV(down, key.down);
		SET_BOOL_IV(repeat, key.repeat);
		SET_INT_IV(window_id, key.windowID);
		SET_INT_IV(which, key.which);
		SET_INT_IV(raw, key.raw);
		SET_SYM_IV(scancode, sdl_scancode_sym(mrb, event.key.scancode));
		SET_SYM_IV(keycode, sdl_keycode_sym(mrb, event.key.key));
		SET_SYM_IV(mod, sdl_kmod_sym(event.key.mod));
		break;
	case SDL_EVENT_TEXT_INPUT:
		COMMON_INIT(text_input);
		SET_INT_IV(window_id, text.windowID);
		SET_CSTRING_IV(text, text.text);
		break;
	case SDL_EVENT_KEYMAP_CHANGED: break;
	case SDL_EVENT_KEYBOARD_ADDED: [[fallthrough]];
	case SDL_EVENT_KEYBOARD_REMOVED:
		COMMON_INIT(keyboard_device);
		SET_INT_IV(which, kdevice.which);
		break;
	case SDL_EVENT_TEXT_EDITING:
		COMMON_INIT(text_editing);
		SET_CSTRING_IV(text, edit.text);
		SET_INT_IV(window_id, edit.windowID);
		SET_INT_IV(start, edit.start);
		SET_INT_IV(length, edit.length);
		break;
	case SDL_EVENT_TEXT_EDITING_CANDIDATES: {
		COMMON_INIT(text_editing_candidates);
		SET_INT_IV(window_id, edit_candidates.windowID);
		SET_INT_IV(selected_candidate,
		    edit_candidates.selected_candidate);
		SET_BOOL_IV(horizontal, edit_candidates.horizontal);
		const auto ary = mrb_ary_new_capa(mrb,
		    event.edit_candidates.num_candidates);
		for (int i = 0; i < event.edit_candidates.num_candidates; ++i) {
			const auto candidate = mrb_str_new_cstr(mrb,
			    event.edit_candidates.candidates[i]);
			mrb_ary_push(mrb, ary, candidate);
		}
		mrb_iv_set(mrb, value, mrb_intern_lit(mrb, "candidates"), ary);
		break;
	}
	case SDL_EVENT_MOUSE_MOTION: {
		COMMON_INIT(mouse_motion);
		SET_INT_IV(window_id, motion.windowID);
		SET_INT_IV(which, motion.which);
		const auto state_ary = mrb_ary_new_capa(mrb, 5);
		const auto buttons = event.motion.state;
		if (buttons & SDL_BUTTON_LMASK) {
			mrb_ary_push(mrb, state_ary,
			    mrb_symbol_value(MRB_SYM(left)));
		}
		if (buttons & SDL_BUTTON_MMASK) {
			mrb_ary_push(mrb, state_ary,
			    mrb_symbol_value(MRB_SYM(middle)));
		}
		if (buttons & SDL_BUTTON_RMASK) {
			mrb_ary_push(mrb, state_ary,
			    mrb_symbol_value(MRB_SYM(right)));
		}
		if (buttons & SDL_BUTTON_X1MASK) {
			mrb_ary_push(mrb, state_ary,
			    mrb_symbol_value(MRB_SYM(x1)));
		}
		if (buttons & SDL_BUTTON_X2MASK) {
			mrb_ary_push(mrb, state_ary,
			    mrb_symbol_value(MRB_SYM(x2)));
		}
		mrb_iv_set(mrb, value, mrb_intern_lit(mrb, "state"), state_ary);
		SET_FLOAT_IV(x, motion.x);
		SET_FLOAT_IV(y, motion.y);
		SET_FLOAT_IV(xrel, motion.xrel);
		SET_FLOAT_IV(yrel, motion.yrel);
		break;
	}
	case SDL_EVENT_MOUSE_BUTTON_DOWN: [[fallthrough]];
	case SDL_EVENT_MOUSE_BUTTON_UP:
		COMMON_INIT(mouse_button);
		SET_INT_IV(window_id, button.windowID);
		SET_INT_IV(which, button.which);
		SET_INT_IV(button, button.button);
		SET_BOOL_IV(pressed, button.down);
		SET_BOOL_IV(clicks, button.clicks);
		SET_FLOAT_IV(x, button.x);
		SET_FLOAT_IV(y, button.y);
		break;
	case SDL_EVENT_MOUSE_WHEEL:
		COMMON_INIT(mouse_wheel);
		SET_INT_IV(window_id, wheel.windowID);
		SET_INT_IV(which, wheel.which);
		SET_FLOAT_IV(x, wheel.x);
		SET_FLOAT_IV(y, wheel.y);
		SET_SYM_IV(direction,
		    event.wheel.direction == SDL_MOUSEWHEEL_NORMAL
			? MRB_SYM(normal)
			: MRB_SYM(flipped));
		SET_FLOAT_IV(mouse_x, wheel.mouse_x);
		SET_FLOAT_IV(mouse_y, wheel.mouse_y);
		break;
	case SDL_EVENT_MOUSE_ADDED: [[fallthrough]];
	case SDL_EVENT_MOUSE_REMOVED:
		COMMON_INIT(mouse_device);
		SET_INT_IV(which, mdevice.which);
		break;
	case SDL_EVENT_JOYSTICK_AXIS_MOTION:
		COMMON_INIT(joy_axis);
		SET_INT_IV(which, jaxis.which);
		SET_INT_IV(axis, jaxis.axis);
		SET_FLOAT_IV(value, jaxis.value / 32768.0f);
		break;
	case SDL_EVENT_JOYSTICK_BALL_MOTION:
		COMMON_INIT(joy_ball);
		SET_INT_IV(which, jball.which);
		SET_INT_IV(ball, jball.ball);
		SET_FLOAT_IV(xrel, jball.xrel / 32768.0f);
		SET_FLOAT_IV(yrel, jball.yrel / 32768.0f);
		break;
	case SDL_EVENT_JOYSTICK_HAT_MOTION:
		COMMON_INIT(joy_hat);
		SET_INT_IV(which, jhat.which);
		SET_INT_IV(hat, jhat.hat);
		SET_SYM_IV(value, sdl_jhat_sym(event.jhat.value));
		break;
	case SDL_EVENT_JOYSTICK_BUTTON_DOWN: [[fallthrough]];
	case SDL_EVENT_JOYSTICK_BUTTON_UP:
		COMMON_INIT(joy_button);
		SET_INT_IV(which, jbutton.which);
		SET_INT_IV(button, jbutton.button);
		SET_BOOL_IV(down, jbutton.down);
		break;
	case SDL_EVENT_JOYSTICK_ADDED:
	case SDL_EVENT_JOYSTICK_REMOVED: [[fallthrough]];
	case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE:
		COMMON_INIT(joy_device);
		SET_INT_IV(which, jdevice.which);
		break;
	case SDL_EVENT_JOYSTICK_BATTERY_UPDATED:
		COMMON_INIT(joy_battery);
		SET_INT_IV(which, jbattery.which);
		SET_SYM_IV(state, sdl_power_sym(event.jbattery.state));
		SET_INT_IV(percent, jbattery.percent);
		break;
	case SDL_EVENT_GAMEPAD_AXIS_MOTION:
		COMMON_INIT(gamepad_axis);
		SET_INT_IV(which, gaxis.which);
		SET_SYM_IV(axis,
		    sdl_gamepad_axis_sym(
			static_cast<SDL_GamepadAxis>(event.gaxis.axis)));
		SET_FLOAT_IV(value, gaxis.value / 32768.0f);
		break;
	case SDL_EVENT_GAMEPAD_BUTTON_DOWN: [[fallthrough]];
	case SDL_EVENT_GAMEPAD_BUTTON_UP:
		COMMON_INIT(gamepad_button);
		SET_INT_IV(which, gbutton.which);
		SET_SYM_IV(button,
		    sdl_gamepad_button_sym(
			static_cast<SDL_GamepadButton>(event.gbutton.button)));
		SET_BOOL_IV(down, gbutton.down);
		break;
	case SDL_EVENT_GAMEPAD_ADDED:
	case SDL_EVENT_GAMEPAD_REMOVED:
	case SDL_EVENT_GAMEPAD_REMAPPED:
	case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE: [[fallthrough]];
	case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
		COMMON_INIT(gamepad_device);
		SET_INT_IV(which, gdevice.which);
		break;
	case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
	case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION: [[fallthrough]];
	case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
		COMMON_INIT(gamepad_touchpad);
		SET_INT_IV(which, gtouchpad.which);
		SET_INT_IV(touchpad, gtouchpad.touchpad);
		SET_INT_IV(finger, gtouchpad.finger);
		SET_FLOAT_IV(x, gtouchpad.x);
		SET_FLOAT_IV(y, gtouchpad.y);
		SET_FLOAT_IV(pressure, gtouchpad.pressure);
		break;
	case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: {
		COMMON_INIT(gamepad_sensor);
		SET_INT_IV(which, gsensor.which);
		SET_INT_IV(sensor, gsensor.sensor);
		auto data_ary = mrb_ary_new_capa(mrb, 3);
		const auto data = event.gsensor.data;
		for (int i = 0; i < 3; ++i) {
			const auto val = mrb_float_value(mrb, data[i]);
			mrb_ary_push(mrb, data_ary, val);
		}
		mrb_iv_set(mrb, value, mrb_intern_lit(mrb, "data"), data_ary);
		SET_INT_IV(timestamp, gsensor.timestamp);
		break;
	}
	case SDL_EVENT_FINGER_DOWN:
	case SDL_EVENT_FINGER_UP:
	case SDL_EVENT_FINGER_MOTION: [[fallthrough]];
	case SDL_EVENT_FINGER_CANCELED:
		COMMON_INIT(touch_finger);
		SET_INT_IV(touch_id, tfinger.touchID);
		SET_INT_IV(finger_id, tfinger.fingerID);
		SET_FLOAT_IV(x, tfinger.x);
		SET_FLOAT_IV(y, tfinger.y);
		SET_FLOAT_IV(dx, tfinger.dx);
		SET_FLOAT_IV(dy, tfinger.dy);
		SET_FLOAT_IV(pressure, tfinger.pressure);
		SET_INT_IV(window_id, tfinger.windowID);
		break;
	case SDL_EVENT_CLIPBOARD_UPDATE: {
		COMMON_INIT(clipboard);
		SET_BOOL_IV(owner, clipboard.owner);
		const auto n = event.clipboard.num_mime_types;
		auto ary = mrb_ary_new_capa(mrb, n);
		for (int i = 0; i < n; ++i) {
			const auto mime = mrb_str_new_cstr(mrb,
			    event.clipboard.mime_types[i]);
			mrb_ary_push(mrb, ary, mime);
		}
		mrb_iv_set(mrb, value, mrb_intern_lit(mrb, "mime_types"), ary);
		break;
	}
	case SDL_EVENT_DROP_FILE:
	case SDL_EVENT_DROP_TEXT:
	case SDL_EVENT_DROP_BEGIN:
	case SDL_EVENT_DROP_COMPLETE: [[fallthrough]];
	case SDL_EVENT_DROP_POSITION:
		COMMON_INIT(drop);
		SET_INT_IV(window_id, drop.windowID);
		SET_FLOAT_IV(x, drop.x);
		SET_FLOAT_IV(y, drop.y);
		SET_CSTRING_IV(source, drop.source);
		SET_CSTRING_IV(data, drop.data);
		break;
	case SDL_EVENT_AUDIO_DEVICE_ADDED:
	case SDL_EVENT_AUDIO_DEVICE_REMOVED: [[fallthrough]];
	case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
		COMMON_INIT(audio_device);
		SET_INT_IV(which, adevice.which);
		SET_BOOL_IV(recording, adevice.recording);
		break;
	case SDL_EVENT_SENSOR_UPDATE: {
		COMMON_INIT(sensor);
		SET_INT_IV(which, sensor.which);
		SET_INT_IV(sensor_timestamp, sensor.sensor_timestamp);
		auto ary = mrb_ary_new_capa(mrb, 6);
		const auto data = event.sensor.data;
		for (int i = 0; i < 6; ++i) {
			const auto val = mrb_float_value(mrb, data[i]);
			mrb_ary_push(mrb, ary, val);
		}
		mrb_iv_set(mrb, value, mrb_intern_lit(mrb, "data"), ary);
		break;
	}
	case SDL_EVENT_PEN_PROXIMITY_IN: [[fallthrough]];
	case SDL_EVENT_PEN_PROXIMITY_OUT:
		COMMON_INIT(pen_proximity);
		SET_INT_IV(window_id, pproximity.windowID);
		SET_INT_IV(which, pproximity.which);
		break;
	case SDL_EVENT_PEN_DOWN: [[fallthrough]];
	case SDL_EVENT_PEN_UP:
		COMMON_INIT(pen_touch);
		SET_INT_IV(window_id, ptouch.windowID);
		SET_INT_IV(which, ptouch.which);
		SET_SYM_IV(pen_state,
		    sdl_pen_input_sym(event.ptouch.pen_state));
		SET_FLOAT_IV(x, ptouch.x);
		SET_FLOAT_IV(y, ptouch.y);
		SET_BOOL_IV(eraser, ptouch.eraser);
		SET_BOOL_IV(down, ptouch.down);
		break;
	case SDL_EVENT_PEN_BUTTON_DOWN: [[fallthrough]];
	case SDL_EVENT_PEN_BUTTON_UP:
		COMMON_INIT(pen_button);
		SET_INT_IV(window_id, pbutton.windowID);
		SET_INT_IV(which, pbutton.which);
		SET_SYM_IV(pen_state,
		    sdl_pen_input_sym(event.pbutton.pen_state));
		SET_INT_IV(button, pbutton.button);
		SET_BOOL_IV(down, pbutton.down);
		break;
	case SDL_EVENT_PEN_MOTION:
		COMMON_INIT(pen_motion);
		SET_INT_IV(window_id, pmotion.windowID);
		SET_INT_IV(which, pmotion.which);
		SET_SYM_IV(pen_state,
		    sdl_pen_input_sym(event.pmotion.pen_state));
		SET_FLOAT_IV(x, pmotion.x);
		SET_FLOAT_IV(y, pmotion.y);
		break;
	case SDL_EVENT_PEN_AXIS:
		COMMON_INIT(pen_axis);
		SET_INT_IV(window_id, paxis.windowID);
		SET_INT_IV(which, paxis.which);
		SET_SYM_IV(pen_state, sdl_pen_input_sym(event.paxis.pen_state));
		SET_FLOAT_IV(x, paxis.x);
		SET_FLOAT_IV(y, paxis.y);
		SET_SYM_IV(axis, sdl_pen_axis_sym(event.paxis.axis));
		SET_FLOAT_IV(value, paxis.value);
		break;
	case SDL_EVENT_CAMERA_DEVICE_ADDED:
	case SDL_EVENT_CAMERA_DEVICE_REMOVED:
	case SDL_EVENT_CAMERA_DEVICE_APPROVED: [[fallthrough]];
	case SDL_EVENT_CAMERA_DEVICE_DENIED:
		COMMON_INIT(camera_device);
		SET_INT_IV(which, cdevice.which);
		break;
	case SDL_EVENT_RENDER_TARGETS_RESET:
	case SDL_EVENT_RENDER_DEVICE_RESET: [[fallthrough]];
	case SDL_EVENT_RENDER_DEVICE_LOST:
		COMMON_INIT(render);
		SET_INT_IV(window_id, render.windowID);
		break;
	default: {
		mrb_gc_arena_restore(mrb, arena_index);
		const auto error
		    = std::format("Unexpected event type {}", event.type);
		throw std::runtime_error(error);
	}
	}
#undef COMMON_INIT
#undef SET_INT_IV
#undef SET_BOOL_IV
#undef SET_CSTRING_IV
#undef SET_SYM_IV
#undef SET_FLOAT_IV

	return value;
}

void
euler::game::init_game_event(util::Reference<State> state)
{
	(void)state;

}