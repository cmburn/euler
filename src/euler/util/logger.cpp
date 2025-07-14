/* SPDX-License-Identifier: ISC */

#include "euler/util/logger.h"

#include "spdlog/pattern_formatter.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"

using Severity = euler::util::Logger::Severity;

struct euler::util::Logger::logger final : spdlog::logger {
	using spdlog::logger::logger;
	template <typename... Args>
	logger(Args &&...args)
	    : spdlog::logger(std::forward<Args>(args)...)
	{
	}
};

static std::shared_ptr<spdlog::sinks::stdout_sink_mt> stdout_sink;
static std::shared_ptr<spdlog::sinks::stderr_sink_mt> stderr_sink;
static std::unique_ptr<spdlog::pattern_formatter> formatter;
static std::once_flag logger_once;

static std::string_view
severity_to_string(const Severity severity)
{
	switch (severity) {
	case Severity::Debug: return "DEBUG";
	case Severity::Info: return "INFO";
	case Severity::Warn: return "WARN";
	case Severity::Error: return "ERROR";
	case Severity::Fatal: return "FATAL";
	case Severity::Unknown: return "ANY";
	}
	return "ANY";
}

class ShortSeverityFlagFormatter final : public spdlog::custom_flag_formatter {
public:
	~ShortSeverityFlagFormatter() override = default;

	void
	format(const spdlog::details::log_msg &msg, const std::tm &,
	    spdlog::memory_buf_t &dest) override
	{
		auto sev = static_cast<Severity>(msg.level);
		auto str = severity_to_string(sev);
		dest.append(str.data(), str.data() + 1);
	}

	std::unique_ptr<custom_flag_formatter>
	clone() const override
	{
		return std::make_unique<ShortSeverityFlagFormatter>();
	}
};

class SeverityFlagFormatter final : public spdlog::custom_flag_formatter {
public:
	~SeverityFlagFormatter() override = default;

	void
	format(const spdlog::details::log_msg &msg, const std::tm &,
	    spdlog::memory_buf_t &dest) override
	{
		auto sev = static_cast<Severity>(msg.level);
		auto str = severity_to_string(sev);
		dest.append(str.data(), str.data() + str.size());
	}

	std::unique_ptr<custom_flag_formatter>
	clone() const override
	{
		return std::make_unique<SeverityFlagFormatter>();
	}
};

static std::string
formatter_pattern(const std::string_view datetime = "%Y-%m-%dT%H:%M:%S.%06F")
{
	return std::format("%Q, [{} #%P] %5q -- %n: %v", datetime);
}

static void
populate_static()
{
	stdout_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
	stderr_sink = std::make_shared<spdlog::sinks::stderr_sink_mt>();
	auto f = std::make_unique<spdlog::pattern_formatter>();
	f->add_flag<ShortSeverityFlagFormatter>('Q');
	f->add_flag<SeverityFlagFormatter>('q');
	f->set_pattern(formatter_pattern());
	formatter = std::move(f);
}

static spdlog::level::level_enum
to_spdlog_level(const Severity severity)
{
	return static_cast<spdlog::level::level_enum>(severity);
}

template <typename T>
std::shared_ptr<euler::util::Logger::logger>
euler::util::Logger::make_logger(const std::string_view progname,
    const Severity severity, std::shared_ptr<T> sink)
{
	auto log = std::make_shared<logger>(progname.data(), sink);
	log->set_level(to_spdlog_level(severity));
	log->set_formatter(formatter->clone());
	return log;
}

euler::util::Logger::Logger(Reference<State> state, const Severity severity,
    const std::string_view progname, const std::string_view datetime_format)
    : Object(state)
{
	std::call_once(logger_once, populate_static);
	_progname = progname;
	_severity = severity;
	_datetime_format = datetime_format;
	_out_log = make_logger<spdlog::sinks::stdout_sink_mt>(progname,
	    severity, stdout_sink);
	_err_log = make_logger<spdlog::sinks::stderr_sink_mt>(progname,
	    severity, stderr_sink);
}

void
euler::util::Logger::write_log(const Severity severity,
    const std::string_view message) const
{
	if (severity < level()) return;
	const auto &log = severity < Severity::Warn ? _out_log : _err_log;
	log->log(to_spdlog_level(severity), message);
}