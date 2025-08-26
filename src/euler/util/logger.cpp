/* SPDX-License-Identifier: ISC */

#include <cassert>
#include <filesystem>
#include <mutex>
#include <ranges>
#include <unordered_map>

#include <SDL3/SDL_time.h>
#include <spdlog/spdlog.h>

#include "config.h"
#include "euler/util/logger.h"
#include "euler/util/state.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using Level = euler::util::Logger::Severity;

static Level
from_spdlog_level(const spdlog::level::level_enum level)
{
	switch (level) {
	case spdlog::level::trace: return Level::Trace;
	case spdlog::level::debug: return Level::Debug;
	case spdlog::level::info: return Level::Info;
	case spdlog::level::warn: return Level::Warn;
	case spdlog::level::err: return Level::Error;
	case spdlog::level::critical: return Level::Critical;
	case spdlog::level::off: return Level::Off;
	default: throw std::runtime_error("Unknown log level");
	}
}

static spdlog::level::level_enum
to_spdlog_level(const Level level)
{
	switch (level) {
	case Level::Trace: return spdlog::level::trace;
	case Level::Debug: return spdlog::level::debug;
	case Level::Info: return spdlog::level::info;
	case Level::Warn: return spdlog::level::warn;
	case Level::Error: return spdlog::level::err;
	case Level::Critical: return spdlog::level::critical;
	case Level::Off: return spdlog::level::off;
	default: throw std::runtime_error("Unknown log level");
	}
}

struct euler::util::Logger::Sink::impl {
	impl(std::shared_ptr<spdlog::sinks::sink> &&sink)
	    : sink(std::move(sink))
	{
		assert(this->sink != nullptr);
	}
	std::shared_ptr<spdlog::sinks::sink> sink;
};

struct euler::util::Logger::impl {
	impl(std::shared_ptr<spdlog::logger> &&log)
	    : log(std::move(log))
	{
		assert(this->log != nullptr);
	}
	std::shared_ptr<spdlog::logger> log;
};

euler::util::Logger::Severity
euler::util::Logger::coerce_severity(severity_enum_t level)
{
	switch (static_cast<Severity>(level)) {
	case Severity::Trace: return Severity::Trace;
	case Severity::Debug: return Severity::Debug;
	case Severity::Info: return Severity::Info;
	case Severity::Warn: return Severity::Warn;
	case Severity::Error: return Severity::Error;
	case Severity::Critical: return Severity::Critical;
	default: return Severity::Unknown;
	}
}

euler::util::Logger::Sink::~Sink() = default;
euler::util::Logger::Sink::Sink(const std::filesystem::path &path)
{
	_impl = std::make_shared<impl>(
	    std::make_shared<spdlog::sinks::basic_file_sink_mt>(path.string(),
		false));
}

void
euler::util::Logger::Sink::set_severity(Severity level)
{
	_impl->sink->set_level(to_spdlog_level(level));
}

euler::util::Logger::Severity
euler::util::Logger::Sink::severity() const
{
	return from_spdlog_level(_impl->sink->level());
}

euler::util::Logger::Sink::Sink(std::shared_ptr<impl> &&sink)
    : _impl(std::move(sink))
{
}

euler::util::Logger::Sink
euler::util::Logger::stdout_sink()
{
	auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	return Sink(std::make_shared<Sink::impl>(std::move(sink)));
}

euler::util::Logger::Sink
euler::util::Logger::stderr_sink()
{
	auto sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
	sink->set_level(spdlog::level::warn);
	return Sink(std::make_shared<Sink::impl>(std::move(sink)));
}
std::vector<euler::util::Logger::Sink>
euler::util::Logger::default_sinks()
{
	return { stdout_sink(), stderr_sink() };
}

euler::util::Logger::Logger(const std::vector<Sink> &sinks,
    std::string_view progname, Severity level)
    : _progname(progname)
    , _level(level)
{
	std::vector<std::shared_ptr<spdlog::sinks::sink>> spd_sinks;
	spd_sinks.reserve(sinks.size());
	for (const auto &sink : sinks) spd_sinks.push_back(sink._impl->sink);
	auto log = std::make_shared<spdlog::logger>(_progname,
	    spd_sinks.begin(), spd_sinks.end());
	spdlog::details::registry::instance().initialize_logger(log);
	_impl = std::make_shared<impl>(std::move(log));
}

void
euler::util::Logger::write_log(const Severity level,
    const std::string &message) const
{
	_impl->log->log(to_spdlog_level(level), message);
}
