/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_LOGGER_H
#define EULER_UTIL_LOGGER_H

#include <filesystem>
#include <format>
#include <memory>
#include <string_view>
#include <vector>

#include "euler/util/object.h"

namespace euler::util {
class Logger final : public Object {
public:
	enum class Severity {
		Invalid,
		Trace,
		Verbose,
		Debug,
		Info,
		Warn,
		Error,
		Critical,
	};

	class Sink final : public Object {
		friend class Logger;

	public:
		~Sink() override;

	private:
		Sink(FILE *output);
		void write(std::string_view str) const;
		mutable std::mutex _mutex;
		std::optional<std::filesystem::path> _path;
		FILE *_file;
	};

	static Reference<Sink> file_sink(const std::filesystem::path &);
	static Reference<Sink> stdout_sink();
	static Reference<Sink> stderr_sink();

	~Logger() override = default;

	Logger(Reference<State> state, const Severity severity,
	    const std::string_view progname,
	    Reference<Sink> output_sink = stdout_sink(),
	    Reference<Sink> error_sink = stderr_sink());

	/* Duplicates the sinks and state from the other logger */
	Logger(Reference<Logger> other, std::string_view progname);

	template <typename... Args>
	void
	add(const Severity severity, const std::format_string<Args...> message,
	    Args &&...args) const
	{
		if (!should_log(severity)) return;
		auto str = std::format(message, std::forward<Args>(args)...);
		write_log(severity, str);
	}

	template <typename... Args>
	void
	trace(const std::format_string<Args...> message, Args &&...args) const
	{
		add(Severity::Trace, message, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void
	verbose(const std::format_string<Args...> message, Args &&...args) const
	{
		add(Severity::Verbose, message, std::forward<Args>(args)...);
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
	critical(const std::format_string<Args...> message,
	    Args &&...args) const
	{
		add(Severity::Critical, message, std::forward<Args>(args)...);
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
	severity() const
	{
		std::lock_guard lock(_severity_mutex);
		return _severity;
	}

	void
	set_severity(const Severity severity)
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

	bool
	should_log(const Severity severity) const
	{
		return severity >= this->severity()
		    || severity >= Severity::Critical;
	}

private:
	void write_log(Severity severity, std::string_view message) const;
	Reference<Sink> _output_sink;
	Reference<Sink> _error_sink;
	Severity _severity = Severity::Info;
	mutable std::mutex _severity_mutex;
	std::string _progname = "euler";
	mutable std::mutex _progname_mutex;
};

} /* namespace Euler::Util */

#endif /* EULER_UTIL_LOGGER_H */
