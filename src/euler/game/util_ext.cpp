/* SPDX-License-Identifier: ISC */

#include "euler/game/util_ext.h"

#include "euler/game/ext.h"
#include "mruby/string.h"

using namespace euler::util;
using namespace euler::game;
using Modules = euler::game::State::Modules;

/* TODO: Config and Version classes are both non-object structs, need to be
 *       handled differently. */

static constexpr auto LOGGER_TYPE = REFERENCE_TYPE(euler::util::Logger);
static constexpr auto LOGGER_SINK_TYPE = DATA_TYPE(euler::util::Logger::Sink);
static constexpr auto STORAGE_TYPE = REFERENCE_TYPE(euler::util::Storage);
static constexpr auto CONFIG_TYPE = DATA_TYPE(euler::util::Config);
static constexpr auto VERSION_TYPE = DATA_TYPE(euler::util::Version);

static mrb_value
logger_severity(mrb_state *mrb, const mrb_value self_value)
{
	switch (const auto self = unwrap<Logger>(mrb, self_value, &LOGGER_TYPE);
	    self->severity()) {
	case Logger::Severity::Trace: return mrb_symbol_value(MRB_SYM(trace));
	case Logger::Severity::Debug: return mrb_symbol_value(MRB_SYM(debug));
	case Logger::Severity::Info: return mrb_symbol_value(MRB_SYM(info));
	case Logger::Severity::Warn: return mrb_symbol_value(MRB_SYM(warn));
	case Logger::Severity::Error: return mrb_symbol_value(MRB_SYM(error));
	case Logger::Severity::Critical:
		return mrb_symbol_value(MRB_SYM(critical));
	case Logger::Severity::Off: return mrb_symbol_value(MRB_SYM(off));
	default: mrb_raise(mrb, E_ARGUMENT_ERROR, "Unexpected logger level");
	}
}

static Logger::Severity
mrb_value_to_severity(mrb_state *mrb, mrb_value value)
{
	if (mrb_symbol_p(value)) {
		switch (mrb_symbol(value)) {
		case MRB_SYM(trace): return Logger::Severity::Trace;
		case MRB_SYM(verbose): return Logger::Severity::Trace;
		case MRB_SYM(debug): return Logger::Severity::Debug;
		case MRB_SYM(info): return Logger::Severity::Info;
		case MRB_SYM(warn): return Logger::Severity::Warn;
		case MRB_SYM(error): return Logger::Severity::Error;
		case MRB_SYM(critical):
		case MRB_SYM(fatal): return Logger::Severity::Critical;
		case MRB_SYM(off): return Logger::Severity::Off;
		default: break;
		}
	} else if (mrb_integer_p(value)) {
		const auto n = mrb_integer(value);
		return Logger::coerce_severity(n);
	} else if (mrb_string_p(value)) {
		using SeverityMap
		    = std::unordered_map<std::string_view, Logger::Severity>;
		static const SeverityMap STR_TO_SEVERITY = {
			{ "trace", Logger::Severity::Trace },
			{ "verbose", Logger::Severity::Trace },
			{ "debug", Logger::Severity::Debug },
			{ "info", Logger::Severity::Info },
			{ "warn", Logger::Severity::Warn },
			{ "error", Logger::Severity::Error },
			{ "critical", Logger::Severity::Critical },
			{ "fatal", Logger::Severity::Critical },
			{ "off", Logger::Severity::Off },
			{ "unknown", Logger::Severity::Unknown },
		};
		const auto str = mrb_str_to_cstr(mrb, value);
		if (const auto it = STR_TO_SEVERITY.find(str);
		    it != STR_TO_SEVERITY.end()) {
			return it->second;
		}
	}
	mrb_raise(mrb, E_ARGUMENT_ERROR, "Unexpected logger level");
}

static mrb_value
logger_set_severity(mrb_state *mrb, mrb_value self_value)
{
	auto ptr = mrb_data_get_ptr(mrb, self_value, &LOGGER_TYPE);
	auto self = Reference<Logger>::unwrap(ptr);
	mrb_value level_value;
	mrb_get_args(mrb, "o", &level_value);
	const auto level = mrb_value_to_severity(mrb, level_value);
	self->set_severity(level);
	return mrb_nil_value();
}

static mrb_value
logger_log(mrb_state *mrb, mrb_value self_value)
{
	auto self = unwrap<Logger>(mrb, self_value, &LOGGER_TYPE);
	mrb_value level_value;
	mrb_get_args(mrb, "o", &level_value);
	const auto level = mrb_value_to_severity(mrb, level_value);
	mrb_value message;
	mrb_get_args(mrb, "S", &message);
	self->log(level, mrb_str_to_cstr(mrb, message));
	return mrb_nil_value();
}

template <Logger::Severity Level>
static mrb_value
logger_log_with_severity(mrb_state *mrb, mrb_value self_value)
{
	auto self = unwrap<Logger>(mrb, self_value, &LOGGER_TYPE);
	mrb_value message;
	mrb_get_args(mrb, "S", &message);
	self->log(Level, mrb_str_to_cstr(mrb, message));
	return mrb_nil_value();
}

static mrb_value
logger_progname(mrb_state *mrb, const mrb_value self_value)
{
	const auto self = unwrap<Logger>(mrb, self_value, &LOGGER_TYPE);
	return mrb_str_new_cstr(mrb, self->progname().c_str());
}

static mrb_value
logger_sink_set_severity(mrb_state *mrb, mrb_value self_value)
{
	const auto level_value = mrb_get_arg1(mrb);
	const auto level = mrb_value_to_severity(mrb, level_value);
	auto self = unwrap<Logger::Sink>(mrb, self_value, &LOGGER_SINK_TYPE);
	self->set_severity(level);
}

static void
init_logger_sink(mrb_state *mrb, Modules &mod)
{
	mod.util.logger.sink = mrb_define_class_under(mrb,
	    mod.util.logger.klass, "Sink", mrb->object_class);
	const auto sink = mod.util.logger.sink;
	mrb_define_method(mrb, sink, "severity", logger_severity, 0);
	mrb_define_method(mrb, sink, "severity=", logger_sink_set_severity,
	    MRB_ARGS_REQ(1));
}

static void
init_logger(mrb_state *mrb, Modules &mod)
{
	mod.util.logger.klass = mrb_define_class_under(mrb, mod.util.module,
	    "Logger", mrb->object_class);
	const auto log = mod.util.logger.klass;
	mrb_define_class_method(mrb, log, "severity", logger_severity, 0);
	mrb_define_class_method(mrb, log, "severity=", logger_set_severity,
	    MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, log, "progname", logger_progname, 0);
	mrb_define_class_method(mrb, log, "log", logger_log, MRB_ARGS_REQ(2));
	mrb_define_class_method(mrb, log, "trace",
	    logger_log_with_severity<Logger::Severity::Trace>, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, log, "verbose",
	    logger_log_with_severity<Logger::Severity::Verbose>,
	    MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, log, "debug",
	    logger_log_with_severity<Logger::Severity::Debug>, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, log, "info",
	    logger_log_with_severity<Logger::Severity::Info>, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, log, "warn",
	    logger_log_with_severity<Logger::Severity::Warn>, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, log, "error",
	    logger_log_with_severity<Logger::Severity::Error>, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, log, "critical",
	    logger_log_with_severity<Logger::Severity::Critical>,
	    MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, log, "fatal",
	    logger_log_with_severity<Logger::Severity::Fatal>, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, log, "off",
	    logger_log_with_severity<Logger::Severity::Off>, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, log, "unknown",
	    logger_log_with_severity<Logger::Severity::Off>, MRB_ARGS_REQ(1));
}

static void
init_storage(mrb_state *mrb, Modules &mod)
{

}

void
euler::game::init_util(util::Reference<State> state)
{
	state->log()->info("Initializing Euler::Util...");
	auto mrb = state->mrb();
	auto &mod = state->euler();
	mod.util.module = mrb_define_module_under(mrb, mod.module, "Util");
	init_logger(mrb, mod);
	init_storage(mrb, mod);
	state->log()->info("Euler::Util initialized");
}