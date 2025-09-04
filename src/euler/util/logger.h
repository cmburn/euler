/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_LOGGER_H
#define EULER_UTIL_LOGGER_H

#include <cassert>
#include <filesystem>
#include <format>
#include <memory>
#include <string_view>
#include <vector>

#include "euler/util/object.h"

namespace euler::util {

class Logger final : public Object {
	friend class State;

public:
	enum class Severity {
		Debug,
		Info,
		Warn,
		Error,
		Fatal,
		Unknown,
	};

	using severity_enum_t = std::underlying_type_t<Severity>;

	static Severity coerce_severity(severity_enum_t level);

	class Sink {
		friend class Logger;

	public:
		~Sink();
		Sink(const std::filesystem::path &path);
		void set_severity(Severity level);
		Severity severity() const;

	private:
		struct impl;
		Sink(std::shared_ptr<impl> &&sink);
		std::shared_ptr<impl> _impl;
	};

	static Sink stdout_sink();
	static Sink stderr_sink();

	static constexpr auto DEFAULT_LEVEL = Severity::Info;
	static constexpr std::string_view DEFAULT_PROGNAME = "euler";

	static std::vector<Sink> default_sinks();

	Logger(const std::vector<Sink> &sinks = default_sinks(),
	    std::string_view progname = DEFAULT_PROGNAME,
	    Severity level = DEFAULT_LEVEL);
	Logger(std::string_view progname = DEFAULT_PROGNAME,
	    Severity level = DEFAULT_LEVEL)
	    : Logger(default_sinks(), progname, level)
	{
	}
	Logger(const Severity level)
	    : Logger(default_sinks(), DEFAULT_PROGNAME, level)
	{
	}

	Severity
	severity() const
	{
		std::lock_guard lock(_level_mutex);
		return _level;
	}

	void
	set_severity(const Severity level)
	{
		std::lock_guard lock(_level_mutex);
		_level = level;
	}

	const std::string &
	progname() const
	{
		return _progname;
	}

	template <typename... Args>
	void
	log(Severity level, const std::format_string<Args...> &message,
	    Args &&...args) const
	{
		if (level < _level) return;
		auto str = std::format(message, std::forward<Args>(args)...);
		write_log(level, str);
	}

	template <typename... Args>
	void
	debug(const std::format_string<Args...> &message, Args &&...args) const
	{
		log(Severity::Debug, message, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void
	info(const std::format_string<Args...> &message, Args &&...args) const
	{
		log(Severity::Info, message, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void
	warn(const std::format_string<Args...> &message, Args &&...args) const
	{
		log(Severity::Warn, message, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void
	error(const std::format_string<Args...> &message, Args &&...args) const
	{
		log(Severity::Error, message, std::forward<Args>(args)...);
	}

	template <typename... Args>
	[[noreturn]] void
	fatal(const std::format_string<Args...> &message,
	    Args &&...args) const
	{
		log(Severity::Fatal, message, std::forward<Args>(args)...);
		std::abort();
	}

private:
	void write_log(Severity level, const std::string &message) const;
	struct impl;
	std::shared_ptr<impl> _impl;
	std::string _progname = "euler";
	mutable std::mutex _level_mutex;
	Severity _level = DEFAULT_LEVEL;
};

} /* namespace Euler::Util */

#endif /* EULER_UTIL_LOGGER_H */
