/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_GAME_STATE_H
#define EULER_GAME_STATE_H

#include <vector>
#include <filesystem>

#include <mruby.h>
#include <unordered_set>

#include "euler/util/state.h"

namespace euler::game {
class State : public util::State {
public:
	struct Config {
		/* argv[0] */
		std::string progname;
		util::Logger::Severity log_level = util::Logger::Severity::Info;
		std::filesystem::path module_path;
		std::optional<std::filesystem::path> config_path;
		std::vector<std::filesystem::path> load_path;
		std::vector<std::string_view> args;
		Config(int argc, char **argv);
	};

	State(int argc, char **argv);
	State(const Config &);
	bool require(std::string_view path);
	~State() override;

private:
	mrb_state *_state = nullptr;
	util::Reference<util::Logger> _log;
	util::Reference<util::Storage> _storage;
	std::unordered_set<std::string> _loaded_modules;
};
} /* namespace euler::game */

#endif /* EULER_GAME_STATE_H */
