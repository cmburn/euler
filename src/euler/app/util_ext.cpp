/* SPDX-License-Identifier: ISC */

#include "euler/app/util_ext.h"

#include "euler/app/ext.h"

#include <mruby/array.h>
#include <mruby/string.h>

#include "../../../extern/mruby/include/mruby/class.h"

using namespace euler::util;
using namespace euler::app;
using Modules = euler::app::State::Modules;

extern const mrb_data_type euler::app::LOGGER_TYPE
    = MAKE_REFERENCE_TYPE(euler::util::Logger);
extern const mrb_data_type euler::app::LOGGER_SINK_TYPE
    = MAKE_DATA_TYPE(euler::util::Logger::Sink);
extern const mrb_data_type euler::app::STORAGE_TYPE
    = MAKE_REFERENCE_TYPE(euler::util::Storage);
extern const mrb_data_type euler::app::CONFIG_TYPE
    = MAKE_DATA_TYPE(euler::util::Config);
extern const mrb_data_type euler::app::VERSION_TYPE
    = MAKE_DATA_TYPE(euler::util::Version);

/* TODO: Config and Version classes are both non-object structs, need to be
 *       handled differently. */

static mrb_value
logger_severity(mrb_state *mrb, const mrb_value self_value)
{
	switch (const auto self = unwrap<Logger>(mrb, self_value, &LOGGER_TYPE);
	    self->severity()) {
	case Logger::Severity::Debug: return mrb_symbol_value(MRB_SYM(debug));
	case Logger::Severity::Info: return mrb_symbol_value(MRB_SYM(info));
	case Logger::Severity::Warn: return mrb_symbol_value(MRB_SYM(warn));
	case Logger::Severity::Error: return mrb_symbol_value(MRB_SYM(error));
	case Logger::Severity::Fatal:
		return mrb_symbol_value(MRB_SYM(fatal));
	case Logger::Severity::Unknown: return mrb_symbol_value(MRB_SYM(unknown));
	default: mrb_raise(mrb, E_ARGUMENT_ERROR, "Unexpected logger level");
	}
}

static Logger::Severity
mrb_value_to_severity(mrb_state *mrb, mrb_value value)
{
	if (mrb_symbol_p(value)) {
		switch (mrb_symbol(value)) {
		case MRB_SYM(debug): return Logger::Severity::Debug;
		case MRB_SYM(info): return Logger::Severity::Info;
		case MRB_SYM(warn): return Logger::Severity::Warn;
		case MRB_SYM(error): return Logger::Severity::Error;
		case MRB_SYM(fatal): return Logger::Severity::Fatal;
		case MRB_SYM(unknown): return Logger::Severity::Unknown;
		default: break;
		}
	} else if (mrb_integer_p(value)) {
		const auto n = mrb_integer(value);
		return Logger::coerce_severity(n);
	} else if (mrb_string_p(value)) {
		using SeverityMap
		    = std::unordered_map<std::string_view, Logger::Severity>;
		static const SeverityMap STR_TO_SEVERITY = {
			{ "debug", Logger::Severity::Debug },
			{ "info", Logger::Severity::Info },
			{ "warn", Logger::Severity::Warn },
			{ "error", Logger::Severity::Error },
			{ "fatal", Logger::Severity::Fatal },
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
	const auto self = unwrap<Logger>(mrb, self_value, &LOGGER_TYPE);
	mrb_value level_value;
	mrb_get_args(mrb, "o", &level_value);
	const auto level = mrb_value_to_severity(mrb, level_value);
	mrb_value message;
	mrb_get_args(mrb, "S", &message);
	self->log(level, "{}", mrb_str_to_cstr(mrb, message));
	return mrb_nil_value();
}

template <Logger::Severity Level>
static mrb_value
logger_log_with_severity(mrb_state *mrb, mrb_value self_value)
{
	auto self = unwrap<Logger>(mrb, self_value, &LOGGER_TYPE);
	mrb_value message;
	mrb_get_args(mrb, "S", &message);
	self->log(Level, "{}", mrb_str_to_cstr(mrb, message));
	return mrb_nil_value();
}

static mrb_value
logger_progname(mrb_state *mrb, const mrb_value self_value)
{
	const auto self = unwrap<Logger>(mrb, self_value, &LOGGER_TYPE);
	return mrb_str_new_cstr(mrb, self->progname().c_str());
}

static void
init_logger(mrb_state *mrb, Modules &mod)
{
	mod.util.logger.klass = mrb_define_class_under(mrb, mod.util.module,
	    "Logger", mrb->object_class);
	const auto log = mod.util.logger.klass;
	MRB_SET_INSTANCE_TT(log, MRB_TT_CDATA);
	mrb_define_method(mrb, log, "severity", logger_severity, 0);
	mrb_define_method(mrb, log, "severity=", logger_set_severity,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, log, "progname", logger_progname, 0);
	mrb_define_method(mrb, log, "log", logger_log, MRB_ARGS_REQ(2));
	mrb_define_method(mrb, log, "debug",
	    logger_log_with_severity<Logger::Severity::Debug>, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, log, "info",
	    logger_log_with_severity<Logger::Severity::Info>, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, log, "warn",
	    logger_log_with_severity<Logger::Severity::Warn>, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, log, "error",
	    logger_log_with_severity<Logger::Severity::Error>, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, log, "fatal",
	    logger_log_with_severity<Logger::Severity::Fatal>, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, log, "unknown",
	    logger_log_with_severity<Logger::Severity::Unknown>, MRB_ARGS_REQ(1));
}

static mrb_value
storage_ready(mrb_state *mrb, const mrb_value self_value)
{
	const auto self = unwrap<Storage>(mrb, self_value, &STORAGE_TYPE);
	return mrb_bool_value(self->ready());
}

static mrb_value
storage_file_size(mrb_state *mrb, const mrb_value self_value)
{
	const auto self = unwrap<Storage>(mrb, self_value, &STORAGE_TYPE);
	char *path;
	mrb_get_args(mrb, "z", &path);
	try {
		const auto size = self->file_size(path);
		return mrb_fixnum_value(size);
	} catch (const std::exception &e) {
		mrb_raise(mrb, E_RUNTIME_ERROR, e.what());
	}
}

static mrb_value
storage_read_file(mrb_state *mrb, const mrb_value self_value)
{
	const auto self = unwrap<Storage>(mrb, self_value, &STORAGE_TYPE);
	char *path_cstr;
	mrb_get_args(mrb, "z", &path_cstr);
	const auto path = std::string(path_cstr);
	try {
		const auto content = self->read_file(path);
		return mrb_str_new_cstr(mrb, content.c_str());
	} catch (const std::exception &e) {
		mrb_raise(mrb, E_RUNTIME_ERROR, e.what());
	}
}

static mrb_value
storage_write_file(mrb_state *mrb, const mrb_value self_value)
{
	const auto self = unwrap<Storage>(mrb, self_value, &STORAGE_TYPE);
	char *path;
	mrb_value content_value;
	mrb_get_args(mrb, "zS", &path, &content_value);
	const auto content = std::string_view(RSTRING_PTR(content_value),
	    RSTRING_LEN(content_value));
	try {
		self->write_file(path, content);
		return mrb_nil_value();
	} catch (const std::exception &e) {
		mrb_raise(mrb, E_RUNTIME_ERROR, e.what());
	}
}

static mrb_value
storage_create_directory(mrb_state *mrb, const mrb_value self_value)
{
	const auto self = unwrap<Storage>(mrb, self_value, &STORAGE_TYPE);
	char *path;
	mrb_get_args(mrb, "z", &path);
	try {
		self->create_directory(path);
		return mrb_nil_value();
	} catch (const std::exception &e) {
		mrb_raise(mrb, E_RUNTIME_ERROR, e.what());
	}
}

static void
init_storage(mrb_state *mrb, Modules &mod)
{
	mod.util.storage = mrb_define_class_under(mrb, mod.util.module,
	    "Storage", mrb->object_class);
	const auto storage = mod.util.storage;
	MRB_SET_INSTANCE_TT(storage, MRB_TT_CDATA);
	mrb_define_method(mrb, storage, "ready?", storage_ready,
	    MRB_ARGS_NONE());
	mrb_define_method(mrb, storage, "file_size", storage_file_size,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, storage, "read_file", storage_read_file,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, storage, "write_file", storage_write_file,
	    MRB_ARGS_REQ(2));
	mrb_define_method(mrb, storage, "create_directory",
	    storage_create_directory, MRB_ARGS_REQ(1));
}

void
euler::app::init_util(Reference<State> state)
{
	state->log()->info("Initializing Euler::Util...");
	auto mrb = state->mrb();
	auto &mod = state->module();
	mod.util.module = mrb_define_module_under(mrb, mod.module, "Util");
	init_logger(mrb, mod);
	init_storage(mrb, mod);
	state->log()->info("Euler::Util initialized");
}