/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_GAME_STATE_H
#define EULER_GAME_STATE_H

#include <filesystem>
#include <vector>

#include <mruby.h>
#include <thread>
#include <unordered_set>

#include "euler/graphics/window.h"
#include "euler/util/config.h"
#include "euler/util/logger.h"
#include "euler/util/state.h"
#include "euler/util/storage.h"
#include "euler/util/version.h"
#include "euler/vulkan/renderer.h"

namespace euler::game {
static constexpr auto DEFAULT_THREAD_COUNT = util::DEFAULT_THREAD_COUNT;
class State final : public util::State {
public:
	// struct Config {
	// 	std::string entry_file;
	// 	/* argv[0] */
	// 	std::string progname;
	// 	std::string name;
	// 	util::Version version = util::Version(0, 1, 0);
	// 	util::Logger::Severity log_level = util::Logger::Severity::Info;
	// 	std::vector<std::filesystem::path> load_path;
	// 	// unsigned long long num_threads = DEFAULT_THREAD_COUNT;
	// 	static Config parse_args(int argc, char **argv);
	// };

	/* If initialization fails, we log the error and return false. */
	bool initialize();
	/* Main game loop of engine. Must be called on same thread as
	 * initialize(). */
	bool loop(int &exit_code);

	bool require(std::string_view path);

	~State() override = default;

	[[nodiscard]] util::Reference<util::Logger>
	log() const override
	{
		return _log;
	}
	[[nodiscard]] util::Reference<util::Storage>
	storage() const override
	{
		return _storage;
	}

	static util::Reference<State>
	get(mrb_state *mrb)
	{
		return util::unsafe_cast<util::WeakReference<State>>(mrb->ud)
		    .strengthen();
	}

	mrb_state *
	mrb()
	{
		return _state;
	}

	struct Modules {
		RClass *module = nullptr;
		struct {
			RClass *module = nullptr;
			RClass *state = nullptr;
		} game;
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
	euler() const
	{
		return _euler;
	}

	Modules &
	euler()
	{
		return _euler;
	}

private:
	friend util::Reference<State> make_state(const util::Config &config);
	friend util::Reference<State> make_state(int argc, char **argv);

	State(const util::Config &);

	std::optional<std::string_view> exception_string();

	bool init_core();

	/* standard init independent of any user content */
	bool load_core();
	bool load_entry(std::string_view path);
	bool handle_event(const SDL_Event &e);
	bool load_entry_file(std::string_view path);

	bool load_text(std::string_view source, std::string_view data);

	mrb_state *_state = nullptr;
	util::Config _config;
	util::Reference<util::Logger> _log;
	util::Reference<util::Storage> _storage;
	util::Reference<vulkan::Renderer> _renderer;
	util::Reference<graphics::Window> _window;
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
	return make_state(config);
}

} /* namespace euler::game */

#endif /* EULER_GAME_STATE_H */
