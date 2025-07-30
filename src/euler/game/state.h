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
#include "euler/util/state.h"
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

	~State() override;

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

private:
	friend util::Reference<State> make_state(const util::Config &config);
	friend util::Reference<State> make_state(int argc, char **argv);

	State(const util::Config &);

	/* standard init independent of any user content */
	bool load_core();
	bool load(std::string_view path);
	bool handle_event(const SDL_Event &e);

	bool load_text(std::string_view source, std::string_view data);

	mrb_state *_state = nullptr;
	util::Config _config;
	util::Reference<util::Logger> _log;
	util::Reference<util::Storage> _storage;
	util::Reference<vulkan::Renderer> _renderer;
	util::Reference<graphics::Window> _window;
	std::unordered_set<std::string> _loaded_modules;
};

inline util::Reference<State>
make_state(const util::Config &config)
{
	return util::make_reference<State>(config);
}

inline util::Reference<State>
make_state(int argc, char **argv)
{
	const auto config = util::Config::parse_args(argc, argv);
	return util::make_reference<State>(config);
}

} /* namespace euler::game */

#endif /* EULER_GAME_STATE_H */
