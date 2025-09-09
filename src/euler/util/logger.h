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
public:
	enum class Severity {
		Debug,
		Info,
		Warn,
		Error,
		Fatal,
		Unknown,
	};

	enum class Color {
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White,
	};

	using enum_t = std::underlying_type_t<Severity>;
	static constexpr enum_t SEVERITY_COUNT
	    = static_cast<enum_t>(Severity::Unknown) + 1;
	static constexpr enum_t COLOR_COUNT
	    = static_cast<enum_t>(Color::White) + 1;

	using ColorList = std::array<Color, SEVERITY_COUNT>;

	static constexpr ColorList DEFAULT_SEVERITY_COLORS = {
		/* [Severity::Debug]   = */ Color::Green,
		/* [Severity::Info]    = */ Color::Green,
		/* [Severity::Warn]    = */ Color::Yellow,
		/* [Severity::Error]   = */ Color::Red,
		/* [Severity::Fatal]   = */ Color::Magenta,
		/* [Severity::Unknown] = */ Color::White,
	};

	static constexpr ColorList DEFAULT_MESSAGE_COLORS = {
		/* [Severity::Debug]   = */ Color::Cyan,
		/* [Severity::Info]    = */ Color::Cyan,
		/* [Severity::Warn]    = */ Color::Cyan,
		/* [Severity::Error]   = */ Color::Cyan,
		/* [Severity::Fatal]   = */ Color::Red,
		/* [Severity::Unknown] = */ Color::Cyan,
	};

	static constexpr std::array<std::string_view, COLOR_COUNT> COLORS = {
		/* [Color::Black]   = */ "\033[30m",
		/* [Color::Red]     = */ "\033[31m",
		/* [Color::Green]   = */ "\033[32m",
		/* [Color::Yellow]  = */ "\033[33m",
		/* [Color::Blue]    = */ "\033[34m",
		/* [Color::Magenta] = */ "\033[35m",
		/* [Color::Cyan]    = */ "\033[36m",
		/* [Color::White]   = */ "\033[37m",
	};

	static constexpr std::array<std::string_view, SEVERITY_COUNT>
	    SEVERITY_NAMES = {
		    /* [Severity::Debug]   = */ "debug",
		    /* [Severity::Info]    = */ "info",
		    /* [Severity::Warn]    = */ "warn",
		    /* [Severity::Error]   = */ "error",
		    /* [Severity::Fatal]   = */ "fatal",
		    /* [Severity::Unknown] = */ "unknown",
	    };

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
	fatal(const std::format_string<Args...> &message, Args &&...args) const
	{
		log(Severity::Fatal, message, std::forward<Args>(args)...);
		std::abort();
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

	static Severity coerce_severity(enum_t level);

	class Sink {
	public:
		Sink(FILE *output, Severity level = Severity::Info);
		Sink(const std::filesystem::path &path,
		    Severity level = Severity::Info);
		Sink(const std::string &path,
		    const Severity level = Severity::Info)
		    : Sink(std::filesystem::path(path), level)
		{
		}

		~Sink();

	private:
		void write_to(Severity severity, const std::string &msg) const;

		friend class Logger;
		struct PrivateStruct { };
		Sink(FILE *output, Severity level, PrivateStruct);

		Severity _min_level = Severity::Info;
		bool _own_file = false;
		FILE *_output = nullptr;
		mutable std::mutex _mutex;
	};

	static std::shared_ptr<Sink> stdout_sink();
	static std::shared_ptr<Sink> stderr_sink();
	static std::vector<std::shared_ptr<Sink>>
	default_sinks()
	{
		return { stdout_sink(), stderr_sink() };
	}

	void
	set_severity_color(const ColorList &colors)
	{
		std::lock_guard lock(_severity_colors_mutex);
		_severity_colors = colors;
	}

	void
	set_message_color(const ColorList &colors)
	{
		std::lock_guard lock(_message_colors_mutex);
		_message_colors = colors;
	}

	std::string
	subsystem() const
	{
		std::lock_guard lock(_subsystem_mutex);
		return _subsystem;
	}

	std::string
	progname() const
	{
		std::lock_guard lock(_progname_mutex);
		return _progname;
	}

	Reference<Logger> copy(const std::optional<std::string_view> &subsystem
	    = std::nullopt) const;

	Logger(std::string_view progname, std::string_view subsystem,
	    Severity level = Severity::Info,
	    const std::vector<std::shared_ptr<Sink>> &sinks = default_sinks());

private:
	// static constexpr std::string_view BLACK = "\033[30m";
	// static constexpr std::string_view RED = "\033[31m";
	// static constexpr std::string_view GREEN = "\033[32m";
	// static constexpr std::string_view YELLOW = "\033[33m";
	// static constexpr std::string_view BLUE = "\033[34m";
	// static constexpr std::string_view MAGENTA = "\033[35m";
	// static constexpr std::string_view CYAN = "\033[36m";
	// static constexpr std::string_view WHITE = "\033[37m";

	Logger(const Logger &other,
	    const std::optional<std::string_view> &subsystem);

	void write_log(Severity level, const std::string &message) const;
	std::string format_message(Severity level,
	    const std::string &message) const;
	Color message_color(Severity level) const;
	Color severity_color(Severity level) const;
	static std::string_view color_for(Color color);
	static std::string_view severity_name(Severity level);

	std::string _progname;
	mutable std::mutex _progname_mutex;
	std::string _subsystem;
	mutable std::mutex _subsystem_mutex;
	Severity _level = Severity::Info;
	mutable std::mutex _level_mutex;
	ColorList _severity_colors = DEFAULT_SEVERITY_COLORS;
	mutable std::mutex _severity_colors_mutex;
	ColorList _message_colors = DEFAULT_MESSAGE_COLORS;
	mutable std::mutex _message_colors_mutex;
	std::vector<std::shared_ptr<Sink>> _sinks;
	mutable std::mutex _sinks_mutex;
};

} /* namespace Euler::Util */

#endif /* EULER_UTIL_LOGGER_H */
