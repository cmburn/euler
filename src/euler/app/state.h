/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_APP_STATE_H
#define EULER_APP_STATE_H

#include <filesystem>
#include <vector>

#include <mruby.h>
#include <mutex>
#include <thread>
#include <unordered_set>

#include "euler/app/system.h"
#include "euler/graphics/window.h"
#include "euler/gui/window.h"
#include "euler/util/config.h"
#include "euler/util/logger.h"
#include "euler/util/mruby_exception.h"
#include "euler/util/state.h"
#include "euler/util/storage.h"
#include "euler/vulkan/renderer.h"
#include "mruby/throw.h"

namespace euler::app {
static constexpr auto DEFAULT_THREAD_COUNT = util::DEFAULT_THREAD_COUNT;
class State final : public util::State {
public:
	struct Config {
		std::string entry_file;
		/* argv[0] */
		std::string progname;
		std::string name;
		util::Version version = util::Version(0, 1, 0);
		util::Logger::Severity log_level = util::Logger::Severity::Info;
		std::vector<std::filesystem::path> load_path;
		static Config parse_args(int argc, char **argv);
	};

	/* If initialization fails, we log the error and return false. */
	bool initialize();
	/* Main game loop of engine. Must be called on same thread as
	 * initialize(). */
	bool loop(int &exit_code);
	bool require(const char *path);

	~State() override;

	[[nodiscard]] util::Reference<util::Logger>
	log() const override
	{
		return _log;
	}

	[[nodiscard]] util::Reference<util::Storage>
	user_storage() const override
	{
		return _user_storage;
	}

	[[nodiscard]] util::Reference<util::Storage>
	title_storage() const override
	{
		return _title_storage;
	}

	static util::Reference<State>
	get(const mrb_state *mrb)
	{
		return util::unsafe_cast<util::WeakReference<State>>(mrb->ud)
		    .strengthen();
	}

	mrb_state *
	mrb() const override
	{
		return _mrb;
	}

	mrb_value
	self_value() const
	{
		return _attributes.self;
	}

	mrb_value
	system_value() const
	{
		return _attributes.system;
	}

	struct Modules {
		RClass *module = nullptr;
		struct {
			RClass *module = nullptr;
			RClass *state = nullptr;
			RClass *system = nullptr;
			RClass *event = nullptr;
			RClass *display_event = nullptr;
			RClass *window_event = nullptr;
			RClass *keyboard_device_event = nullptr;
			RClass *keyboard_event = nullptr;
			RClass *text_editing_event = nullptr;
			RClass *text_editing_candidates_event = nullptr;
			RClass *text_input_event = nullptr;
			RClass *mouse_device_event = nullptr;
			RClass *mouse_motion_event = nullptr;
			RClass *mouse_button_event = nullptr;
			RClass *mouse_wheel_event = nullptr;
			RClass *joystick_device_event = nullptr;
			RClass *joystick_axis_motion_event = nullptr;
			RClass *joystick_ball_motion_event = nullptr;
			RClass *joystick_hat_motion_event = nullptr;
			RClass *joystick_button_event = nullptr;
			RClass *joystick_battery_updated_event = nullptr;
			RClass *gamepad_device_event = nullptr;
			RClass *gamepad_axis_motion_event = nullptr;
			RClass *gamepad_button_event = nullptr;
			RClass *gamepad_touchpad_event = nullptr;
			RClass *gamepad_sensor_event = nullptr;
			RClass *audio_device_event = nullptr;
			RClass *camera_device_event = nullptr;
			RClass *sensor_event = nullptr;
			RClass *quit_event = nullptr;
			RClass *user_event = nullptr;
			RClass *touch_finger_event = nullptr;
			RClass *pen_proximity_event = nullptr;
			RClass *pen_touch_event = nullptr;
			RClass *pen_motion_event = nullptr;
			RClass *pen_button_event = nullptr;
			RClass *pen_axis_event = nullptr;
			RClass *render_event = nullptr;
			RClass *drop_event = nullptr;
			RClass *clipboard_event = nullptr;
		} app;
		struct {
			RClass *module = nullptr;
			RClass *camera = nullptr;
			RClass *color = nullptr;
			RClass *font = nullptr;
			RClass *polygon = nullptr;
			RClass *text = nullptr;
			RClass *window = nullptr;
		} graphics;
		struct {
			RClass *module = nullptr;
			RClass *button = nullptr;
			RClass *checkbox = nullptr;
			RClass *color_picker = nullptr;
			RClass *element = nullptr;
			RClass *knob = nullptr;
			RClass *progress_bar = nullptr;
			RClass *radio = nullptr;
			RClass *selectable = nullptr;
			RClass *slider = nullptr;
			RClass *widget = nullptr;
		} gui;
		struct {
			RClass *module = nullptr;
		} physics;
		struct {
			RClass *module = nullptr;
			RClass *config = nullptr;
			struct {
				RClass *klass = nullptr;
				RClass *sink = nullptr;
			} logger;
			RClass *storage = nullptr;
			RClass *version = nullptr;
		} util;
		struct {
			RClass *module = nullptr;
		} vulkan;
	};

	[[nodiscard]] const Modules &
	module() const
	{
		return _euler;
	}

	Modules &
	module()
	{
		return _euler;
	}

	mrb_value gv_state();

	[[nodiscard]] std::unique_lock<std::mutex>
	lock_mrb() const override
	{
		return std::unique_lock(_mrb_mutex);
	}

	[[nodiscard]] std::optional<std::unique_lock<std::mutex>>
	try_lock_mrb() const override
	{
		std::optional<std::lock_guard<std::mutex>> guard;
		auto lk = std::unique_lock(_mrb_mutex, std::try_to_lock);
		if (lk.owns_lock()) return lk;
		return std::nullopt;
	}

	util::Reference<System>
	system() const
	{
		return _system;
	}

	void set_ivs();

	void assert_state() const;
	util::MRubyException make_exception(RObject *exc) const override;
	static void wrap_mrb_exception(mrb_state *mrb, RObject *exc);

	template <typename T>
	T
	wrap_call(const std::function<T()> &fn)
	{
		mrb_jmpbuf jmp = {}, *prev_jmp = _mrb->jmp;
		/* ReSharper disable once CppDFALocalValueEscapesFunction */
		_mrb->jmp = &jmp;
		MRB_TRY(mrb()->jmp)
		{
			auto value = fn();
			mrb()->jmp = prev_jmp;
			return value;
		}
		MRB_CATCH(mrb()->jmp)
		{
			mrb()->jmp = prev_jmp;
			throw make_exception(mrb()->exc);
		};
		MRB_END_EXC(mrb()->jmp);
	}

private:
	friend util::Reference<State> make_state(const util::Config &config);
	friend util::Reference<State> make_state(int argc, char **argv);

	State(const util::Config &);

	bool update(int &exit_code);
	bool verify_gv_state();
	bool app_update(float dt);
	bool app_input(const SDL_Event &event);
	bool app_draw();
	bool app_load();
	bool app_quit();

	std::optional<std::string_view> exception_string() const;

	/* standard init independent of any user content */
	bool load_core();
	bool load_entry(std::string_view path);

	bool load_text(std::string_view source, std::string_view data);
	void check_mrb() const;

	[[nodiscard]] util::nthread_t available_threads() const override;
	mrb_state *_mrb = nullptr;
	mutable std::mutex _mrb_mutex;
	struct HaveMethod {
		bool input : 1;
		bool update : 1;
		bool load : 1;
		bool draw : 1;
		bool quit : 1;
		HaveMethod()
		    : input(false)
		    , update(false)
		    , load(false)
		    , draw(false)
		    , quit(false)
		{
		}
	};

	struct {
		mrb_value self = mrb_nil_value();
		mrb_value system = mrb_nil_value();
		mrb_value log = mrb_nil_value();
	} _attributes;
	HaveMethod _methods;
	util::Config _config;
	util::Reference<System> _system;
	util::Reference<util::Logger> _log;
	util::Reference<util::Storage> _user_storage;
	util::Reference<util::Storage> _title_storage;
	util::Reference<vulkan::Renderer> _renderer;
	util::Reference<graphics::Window> _window;
	util::Reference<gui::Window> _gui;
	std::unordered_set<std::string> _loaded_modules;
	Modules _euler;
};

inline util::Reference<State>
make_state(const util::Config &config)
{
	return util::Reference(new State(config));
}

inline util::Reference<State>
make_state(int argc, char **argv)
{
	const auto config = util::Config::parse_args(argc, argv);
	auto state = make_state(config);
	state.increment();
	return state;
}

} /* namespace euler::app */

#endif /* EULER_APP_STATE_H */
