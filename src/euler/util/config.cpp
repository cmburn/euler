/* SPDX-License-Identifier: ISC */

#include "euler/util/config.h"

extern "C" {
#include "euler/util/optparse.h"
}

#include <iostream>
#include <thread>
#include <unordered_map>

static constexpr unsigned long long DEFAULT_THREAD_COUNT
    = euler::util::DEFAULT_THREAD_COUNT;

using Severity = euler::util::Logger::Severity;
using SeverityMap = std::unordered_map<std::string_view, Severity>;
static const SeverityMap LOG_LEVELS = {
	{ "debug", Severity::Debug },
	{ "info", Severity::Info },
	{ "warn", Severity::Warn },
	{ "error", Severity::Error },
	{ "fatal", Severity::Fatal },
	{ "unknnown", Severity::Unknown },
};

[[noreturn]] static void
usage(std::string_view progname, const bool is_error = true)
{
	auto &out = is_error ? std::cerr : std::cout;
	out << std::format(R"EOF(
Euler Game Engine {}
usage: {} [options] <file>
Options:
	-I, --include <path>    Add a directory to the load path. '../lib',
				relative to <file>, is added by default if it
				exists. Note that the load path must also
				include assets that are to be loaded by any
				game scripts.
	-V, --version           Print version information and exit
	-c, --config <file>     Load configuration from file
	-d, --directory <dir>   Set the working directory to <dir>. This should
				be the root of the game project.
				(default: present working directory)
	-h, --help              Show this help message and exit
	-l, --log-level <level> Set the log level. (default: info)
				May be one of:
				 - trace
				 - verbose
				 - debug
				 - info
				 - warn
				 - error
				 - critical
	-n, --num-threads <n>   Set the number of threads to use (default: {})
	-q, --quiet             Decrease log level by one
	-v, --verbose           Increase log level by one
Notes:
	<file> should be the entry point of the game. It is expected to create
	an object that inherits from `Euler::Game::State` and assign it to
	`$state`.
)EOF",
	    euler::util::version().to_string(), progname, DEFAULT_THREAD_COUNT)
	    << std::endl;
	exit(is_error ? EXIT_FAILURE : EXIT_SUCCESS);
}

static void
parse_config_file(euler::util::Config &, std::string_view)
{
}

static void
parse_thread_count(euler::util::Config &config, std::string_view opt)
{
	char *endptr;
	auto n = strtoull(opt.data(), &endptr, 10);
	if (*endptr != '\0' || n == 0) {
		std::cerr << "Number of threads must be an unsigned "
			     "integer, unable to parse '"
			  << opt << "'" << std::endl;
		usage(config.progname);
	}
	auto max = std::thread::hardware_concurrency();
	if (max == 0) {
		std::cerr << "Unable to determine the number "
			     "hardware threads, using default "
			     "of 6 threads."
			  << std::endl;
		max = DEFAULT_THREAD_COUNT;
	}
	if (n > max) {
		std::cerr << "Only " << max << " threads are supported, using "
			  << max << " threads instead of " << n << "."
			  << std::endl;
		n = max;
	}
	if (n == 0) {
		std::cerr << "Number of threads must be greater than "
			     "zero, using default of "
			  << DEFAULT_THREAD_COUNT << " threads" << std::endl;
		n = 1;
	}
	config.num_threads = n;
}

euler::util::Config
euler::util::Config::parse_args(int argc, char **argv)
{
	static constexpr struct optparse_long LONGOPTS[] = {
		{
		    .longname = "include",
		    .shortname = 'I',
		    .argtype = OPTPARSE_REQUIRED,
		},
		{
		    .longname = "version",
		    .shortname = 'V',
		    .argtype = OPTPARSE_NONE,
		},
		{
		    .longname = "config",
		    .shortname = 'c',
		    .argtype = OPTPARSE_REQUIRED,
		},
		{
		    .longname = "directory",
		    .shortname = 'd',
		    .argtype = OPTPARSE_REQUIRED,
		},
		{
		    .longname = "help",
		    .shortname = 'h',
		    .argtype = OPTPARSE_NONE,
		},
		{
		    .longname = "log-level",
		    .shortname = 'l',
		    .argtype = OPTPARSE_REQUIRED,
		},
		{
		    .longname = "num-threads",
		    .shortname = 'n',
		    .argtype = OPTPARSE_REQUIRED,
		},
		{
		    .longname = "quiet",
		    .shortname = 'q',
		    .argtype = OPTPARSE_NONE,
		},
		{
		    .longname = "verbose",
		    .shortname = 'v',
		    .argtype = OPTPARSE_NONE,
		},
		{ /* sentinel */ },
	};

	Config out = {
		.progname = std::filesystem::path(argv[0]).filename().string(),
		.entry_file = {},
		.name = "euler",
		.version = Version(0, 1, 0),
		.log_level = Severity::Info,
		.load_path = {},
		.num_threads = DEFAULT_THREAD_COUNT,
	};
	struct optparse options;
	optparse_init(&options, argv);
	int opt;
	while ((opt = optparse_long(&options, LONGOPTS, nullptr)) != -1) {
		switch (opt) {
		case 'I': out.load_path.emplace_back(options.optarg); break;
		case 'V': std::cout << util::version() << std::endl; exit(0);
		case 'c': parse_config_file(out, options.optarg); break;
		case 'h': usage(out.progname, false); break;
		case 'l': {
			if (!LOG_LEVELS.contains(options.optarg)) {
				std::cerr
				    << "Unknown log level: " << options.optarg
				    << std::endl;
				usage(out.progname);
			}
			out.log_level = LOG_LEVELS.at(options.optarg);
			break;
		}
		case 'n': parse_thread_count(out, options.optarg); break;
		case 'q': {
			out.log_level = static_cast<Severity>(
			    static_cast<int>(out.log_level) + 1);
			break;
		}
		case 'v': {
			out.log_level = static_cast<Severity>(
			    static_cast<int>(out.log_level) - 1);
			break;
		}
		default: usage(out.progname);
		}
	}
	out.log_level
	    = std::clamp(out.log_level, Severity::Debug, Severity::Fatal);
	if (argc - options.optind > 1) {
		std::cerr << "Only one entry file can be specified"
			  << std::endl;
		usage(out.progname);
	}
	if (argc - options.optind < 1) {
		std::cerr << "An entry file must be specified" << std::endl;
		usage(out.progname);
	}
	out.entry_file = options.argv[options.optind];
	return out;
}
