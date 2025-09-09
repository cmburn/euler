/* SPDX-License-Identifier: ISC */

#include <cassert>
#include <filesystem>
#include <mutex>
#include <ranges>
#include <unordered_map>

#include <SDL3/SDL_time.h>

#include "euler/util/logger.h"
#include "euler/util/state.h"

using Severity = euler::util::Logger::Severity;
using Color = euler::util::Logger::Color;

euler::util::Logger::Severity
euler::util::Logger::coerce_severity(enum_t level)
{
	switch (static_cast<Severity>(level)) {
	case Severity::Debug: return Severity::Debug;
	case Severity::Info: return Severity::Info;
	case Severity::Warn: return Severity::Warn;
	case Severity::Error: return Severity::Error;
	case Severity::Fatal: return Severity::Fatal;
	default: return Severity::Unknown;
	}
}

euler::util::Logger::Sink::Sink(FILE *output, const Severity level)
    : _min_level(level)
    , _output(output)
{
	assert(output != nullptr);
	assert(output != stdout && "Use Logger::stdout_sink() instead");
	assert(output != stderr && "Use Logger::stderr_sink() instead");
}

euler::util::Logger::Sink::Sink(const std::filesystem::path &path,
    const Severity level)
    : Sink(std::fopen(path.string().c_str(), "a"), level)
{
	if (_output == nullptr) {
		throw std::runtime_error("Failed to open log file: "
		    + path.string() + ": " + std::strerror(errno));
	}
	_own_file = true;
}

euler::util::Logger::Sink::~Sink()
{
	if (_own_file) fclose(_output);
}

void
euler::util::Logger::Sink::write_to(const Severity severity,
    const std::string &msg) const
{
	if (severity < _min_level) return;
	std::lock_guard lock(_mutex);
	fprintf(_output, "%s", msg.c_str());
	fflush(_output);
}

euler::util::Logger::Sink::Sink(FILE *output, const Severity level,
    PrivateStruct)
    : _min_level(level)
    , _output(output)
{
	assert(output != nullptr);
}

std::shared_ptr<euler::util::Logger::Sink>
euler::util::Logger::stdout_sink()
{
	static std::weak_ptr<Sink> instance;
	static std::mutex instance_mutex;
	std::lock_guard lock(instance_mutex);
	std::shared_ptr<Sink> shared = nullptr;
	if (!instance.expired()) {
		shared = instance.lock();
		if (shared != nullptr) return shared;
	}
	Sink *sink = nullptr;
	try {
		/* ReSharper disable once CppDFAMemoryLeak */
		sink
		    = new Sink(stdout, Severity::Debug, Sink::PrivateStruct {});
		shared = std::shared_ptr<Sink>(sink);
		instance = shared;
		/* ReSharper disable once CppDFAMemoryLeak */
		return shared;
	} catch (...) {
		delete sink;
		throw;
	}
}

std::shared_ptr<euler::util::Logger::Sink>
euler::util::Logger::stderr_sink()
{
	static std::weak_ptr<Sink> instance;
	static std::mutex instance_mutex;
	std::lock_guard lock(instance_mutex);
	std::shared_ptr<Sink> shared = nullptr;
	if (!instance.expired()) {
		shared = instance.lock();
		if (shared != nullptr) return shared;
	}
	Sink *sink = nullptr;
	try {
		/* ReSharper disable once CppDFAMemoryLeak */
		sink = new Sink(stderr, Severity::Warn, Sink::PrivateStruct {});
		shared = std::shared_ptr<Sink>(sink);
		instance = shared;
		/* ReSharper disable once CppDFAMemoryLeak */
		return shared;
		/* ReSharper disable once CppDFAUnreachableCode */
	} catch (...) {
		delete sink;
		throw;
	}
}

euler::util::Reference<euler::util::Logger>
euler::util::Logger::copy(
    const std::optional<std::string_view> &subsystem) const
{
	return Reference(new Logger(*this, subsystem));
}

euler::util::Logger::Logger(const std::string_view progname,
    const std::string_view subsystem, const Severity level,
    const std::vector<std::shared_ptr<Sink>> &sinks)
    : _progname(progname)
    , _subsystem(subsystem)
    , _level(level)
    , _severity_colors(DEFAULT_SEVERITY_COLORS)
    , _message_colors(DEFAULT_MESSAGE_COLORS)
    , _sinks(sinks)
{
}

euler::util::Logger::Logger(const Logger &other,
    const std::optional<std::string_view> &subsystem)
{
	{
		std::lock_guard lock(other._progname_mutex);
		_progname = other._progname;
	}
	{
		std::lock_guard lock(other._subsystem_mutex);
		_subsystem = subsystem.value_or(other._subsystem);
	}
	{
		std::lock_guard lock(other._level_mutex);
		_level = other._level;
	}
	{
		std::lock_guard lock(other._sinks_mutex);
		_sinks = other._sinks;
	}
	{
		std::lock_guard lock(other._severity_colors_mutex);
		_severity_colors = other._severity_colors;
	}
	{
		std::lock_guard lock(other._message_colors_mutex);
		_message_colors = other._message_colors;
	}
}

void
euler::util::Logger::write_log(const Severity level,
    const std::string &message) const
{
	if (level < _level) return;
	std::lock_guard lock(_sinks_mutex);
	auto formatted = format_message(level, message);
	for (const auto &s : _sinks) s->write_to(level, formatted);
}

static SDL_DateTime
current_time()
{
	SDL_Time t;
	SDL_GetCurrentTime(&t);
	SDL_DateTime dt;
	SDL_TimeToDateTime(t, &dt, true);
	return dt;
}

static constexpr size_t MAX_TIME_STRING_SIZE = 26;

static void
write_time(std::stringstream &ss)
{
	static constexpr const char *WEEK_DAYS[] = {
		"Sun",
		"Mon",
		"Tue",
		"Wed",
		"Thu",
		"Fri",
		"Sat",
	};
	static constexpr const char *MONTHS[] = {
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec",
	};

	const auto dt = current_time();
	char time_string[MAX_TIME_STRING_SIZE] = { 0 };
	snprintf(time_string, MAX_TIME_STRING_SIZE,
	    "%s %s %i %02d:%02d:%02d %i", WEEK_DAYS[dt.day_of_week],
	    MONTHS[dt.month - 1], dt.day, dt.hour, dt.minute, dt.second,
	    dt.year);
	ss << time_string;
}

std::string
euler::util::Logger::format_message(const Severity level,
    const std::string &message) const
{
	const auto message_color = this->message_color(level);
	const auto severity_color = this->severity_color(level);
	std::stringstream ss;

	ss << color_for(message_color) << "[" << color_for(severity_color);
	write_time(ss);
	ss << color_for(message_color) << "] [" << color_for(severity_color)
	   << severity_name(level) << color_for(message_color) << "]";
	const auto sev_str = severity_name(level);
	{
		std::lock_guard lock(_progname_mutex);
		ss << " [" << color_for(severity_color) << _progname
		<< color_for(message_color) << "::";
	}
	{
		std::lock_guard lock(_subsystem_mutex);
		ss << color_for(severity_color) << _subsystem;
	}
	ss << color_for(message_color) << "] ";
	const auto padding = sizeof("unknown") - sev_str.size();
	for (size_t i = 0; i < padding; i++) ss << ' ';
	ss << message << std::endl;
	ss << color_for(Color::White);
	return ss.str();
}

euler::util::Logger::Color
euler::util::Logger::message_color(Severity level) const
{
	std::lock_guard lock(_message_colors_mutex);
	return _message_colors.at(static_cast<size_t>(level));
}

euler::util::Logger::Color
euler::util::Logger::severity_color(Severity level) const
{
	std::lock_guard lock(_severity_colors_mutex);
	return _severity_colors.at(static_cast<size_t>(level));
}

std::string_view
euler::util::Logger::color_for(Color color)
{
	return COLORS.at(static_cast<size_t>(color));
}

std::string_view
euler::util::Logger::severity_name(Severity level)
{
	const auto idx = coerce_severity(static_cast<enum_t>(level));
	return SEVERITY_NAMES[static_cast<enum_t>(idx)];
}
