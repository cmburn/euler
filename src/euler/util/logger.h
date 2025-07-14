/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_LOGGER_H
#define EULER_UTIL_LOGGER_H

#include <format>
#include <memory>
#include <string_view>

#include "euler/util/object.h"

namespace euler::util {
class Logger final : public Object {
public:
	enum class Severity {
		Debug,
		Info,
		Warn,
		Error,
		Fatal,
		Unknown,
	};

	static constexpr std::string_view DEFAULT_DATETIME_FORMAT
	    = "%Y-%m-%dT%H:%M:%S.%6N";

	Logger(Reference<State>, Severity severity = Severity::Debug,
	    std::string_view progname = "euler",
	    std::string_view datetime_format = DEFAULT_DATETIME_FORMAT);

	Logger(Reference<State> state, const std::string_view progname)
	    : Logger(state, Severity::Debug, progname, DEFAULT_DATETIME_FORMAT)
	{
	}

	template <typename... Args>
	void
	add(const Severity severity, const std::format_string<Args...> message,
	    Args &&...args) const
	{
		auto str = std::format(message, std::forward<Args>(args)...);
		write_log(severity, str);
	}

	template <typename... Args>
	void
	debug(const std::format_string<Args...> message, Args &&...args) const
	{
		add(Severity::Debug, message, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void
	info(const std::format_string<Args...> message, Args &&...args) const
	{
		add(Severity::Info, message, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void
	warn(const std::format_string<Args...> message, Args &&...args) const
	{
		add(Severity::Warn, message, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void
	error(const std::format_string<Args...> message, Args &&...args) const
	{
		add(Severity::Error, message, std::forward<Args>(args)...);
	}

	template <typename... Args>
	[[noreturn]] void
	fatal(const std::format_string<Args...> message, Args &&...args) const
	{
		add(Severity::Fatal, message, std::forward<Args>(args)...);
		std::abort();
	}

	template <typename... Args>
	void
	log(const Severity severity, const std::format_string<Args...> message,
	    Args &&...args) const
	{
		add(severity, message, std::forward<Args>(args)...);
	}

	Severity
	level() const
	{
		std::lock_guard lock(_severity_mutex);
		return _severity;
	}

	void
	set_level(const Severity severity)
	{
		std::lock_guard lock(_severity_mutex);
		_severity = severity;
	}

	std::string
	progname() const
	{
		std::lock_guard lock(_progname_mutex);
		return _progname;
	}

	void
	set_progname(const std::string_view progname)
	{
		std::lock_guard lock(_progname_mutex);
		_progname = progname;
	}

private:
	struct logger;
	template <typename SinkT>
	static std::shared_ptr<logger> make_logger(std::string_view progname,
	    Severity severity, std::shared_ptr<SinkT> sink);

	void write_log(Severity severity, std::string_view message) const;
	std::shared_ptr<logger> _out_log;
	std::shared_ptr<logger> _err_log;
	Severity _severity = Severity::Info;
	mutable std::mutex _severity_mutex;
	std::string _progname = "euler";
	mutable std::mutex _progname_mutex;
	std::string _datetime_format = DEFAULT_DATETIME_FORMAT.data();
	mutable std::mutex _datetime_format_mutex;
};

} /* namespace Euler::Util */

#endif /* EULER_UTIL_LOGGER_H */
