/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_CONFIG_H
#define EULER_UTIL_CONFIG_H

#include <filesystem>

#include "euler/util/logger.h"
#include "euler/util/version.h"

namespace euler::util {
static constexpr unsigned long long DEFAULT_THREAD_COUNT = 6;
struct Config {
	/* argv[0] */
	std::string progname;
	std::string entry_file;
	std::string name;
	util::Version version = util::Version(0, 1, 0);
	util::Logger::Severity log_level = util::Logger::Severity::Info;
	std::vector<std::filesystem::path> load_path;
	unsigned long long num_threads = DEFAULT_THREAD_COUNT;
	static Config parse_args(int argc, char **argv);
};
} /* namespace euler::util */


#endif /* EULER_UTIL_CONFIG_H */

