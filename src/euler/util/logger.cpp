/* SPDX-License-Identifier: ISC */

#include <cassert>
#include <filesystem>
#include <mutex>
#include <ranges>
#include <unordered_map>

#include <SDL3/SDL_time.h>

#include "euler/util/logger.h"
#include "euler/util/state.h"

#define MAX_SEVERITY_LABEL_LENGTH (sizeof("critical") - 1)
#define MAX_TIME_STRING_SIZE 26

static std::unordered_map<std::filesystem::path,
    euler::util::Reference<euler::util::Logger::Sink>>
    files;
static std::mutex files_mutex;
//
// struct euler::util::Logger::Sink::stream {
// 	std::filesystem::path path = {};
// 	mutable std::mutex mutex = {};
// 	FILE *file = nullptr;
// 	stream(std::filesystem::path &&path, FILE *output = nullptr)
// 	    : path(std::move(path))
// 	    , file(output)
// 	{
// 	}
//
// 	void
// 	write(const std::string_view msg) const
// 	{
// 		std::scoped_lock lock(mutex);
// 		fwrite(msg.data(), sizeof(char), msg.size(), file);
// 	}
//
// 	stream(FILE *output)
// 	    : file(output)
// 	{
// 		assert(output == stdout || output == stderr);
// 	}
//
// 	~stream();
// };
//
// euler::util::Logger::Sink::stream::~stream()
// {
// 	if (file == stdout || file == stderr || file == nullptr) return;
// 	fclose(file);
// }
//
euler::util::Logger::Sink::~Sink()
{
	if (_file == stdout || _file == stderr || _file == nullptr) return;
	fclose(_file);
}
//
euler::util::Reference<euler::util::Logger::Sink>
euler::util::Logger::file_sink(const std::filesystem::path &path)
{
	std::scoped_lock lock(files_mutex);
	auto absolute = std::filesystem::absolute(path);
	if (files.contains(absolute)) return files.at(absolute);
	auto file = fopen(absolute.c_str(), "a");
	if (file == nullptr) {
		throw std::runtime_error(
		    "Failed to open log file: " + absolute.string());
	}
	auto output = util::Reference(new Sink(file));
	output->_path = std::move(absolute);
	files[absolute] = output;
	return output;
}
//
euler::util::Reference<euler::util::Logger::Sink>
euler::util::Logger::stdout_sink()
{
	static auto stdout_output = util::Reference(new Sink(stdout));
	return stdout_output;
}

euler::util::Reference<euler::util::Logger::Sink>
euler::util::Logger::stderr_sink()
{
	static auto stderr_output = util::Reference(new Sink(stderr));
	return stderr_output;
}
euler::util::Logger::Logger(Reference<State> state, const Severity severity,
    const std::string_view progname, Reference<Sink> output_sink,
    Reference<Sink> error_sink)
    : Object(state)
    , _output_sink(std::move(output_sink))
    , _error_sink(std::move(error_sink))
    , _severity(severity)
    , _progname(progname)
{
}

static std::string_view
severity_to_string(const euler::util::Logger::Severity severity)
{
	switch (severity) {
	case euler::util::Logger::Severity::Trace: return "trace";
	case euler::util::Logger::Severity::Verbose: return "verbose";
	case euler::util::Logger::Severity::Debug: return "debug";
	case euler::util::Logger::Severity::Info: return "info";
	case euler::util::Logger::Severity::Warn: return "warn";
	case euler::util::Logger::Severity::Error: return "error";
	case euler::util::Logger::Severity::Critical: return "critical";
	default: return "invalid";
	}
}

static void
write_time_string(char *buf)
{
	static const char *WEEK_DAYS[] = {
		"Sun",
		"Mon",
		"Tue",
		"Wed",
		"Thu",
		"Fri",
		"Sat",
	};
	static const char *MONTHS[] = {
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
	SDL_Time time;
	SDL_DateTime dt;

	if (!SDL_GetCurrentTime(&time)
	    || !SDL_TimeToDateTime(time, &dt, true)) {
		memset(buf, '-', MAX_TIME_STRING_SIZE);
		return;
	}

	snprintf(buf, MAX_TIME_STRING_SIZE, "%s %s %i %02d:%02d:%02d %i",
	    WEEK_DAYS[dt.day_of_week], MONTHS[dt.month - 1], dt.day, dt.hour,
	    dt.minute, dt.second, dt.year);
}

static std::string
format_string(const euler::util::Logger::Severity severity,
    const std::string_view progname, const std::string_view message)
{
	char time_buf[MAX_TIME_STRING_SIZE];
	std::stringstream ss;
	write_time_string(time_buf);
	auto sev_str = severity_to_string(severity);
	ss << "[" << time_buf << "] "
	   << "[" << sev_str << "] "
	   << "[" << progname << "] ";
	for (size_t i = (sizeof("critical") - sev_str.size()); i > 0; i--)
		ss << ' ';
	ss << "-- " << message << '\n';
	return ss.str();
}

void
euler::util::Logger::write_log(const Severity severity,
    const std::string_view message) const
{
	assert(should_log(severity));
	const auto str = format_string(severity, progname(), message);
	if (this->severity() >= Severity::Warn) _error_sink->write(str);
	_output_sink->write(str);
}
