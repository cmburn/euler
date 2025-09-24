/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_EXT_H
#define EULER_APP_EXT_H

#include <mruby.h>
#include <mruby/data.h>
#include <mruby/presym.h>

#include "euler/util/object.h"

/* common extension utilities */

namespace euler::app {

/* Standard Euler Object type */
#define MAKE_REFERENCE_TYPE(CLASS)                                             \
	mrb_data_type                                                          \
	{                                                                      \
		.struct_name = #CLASS, .dfree = [](mrb_state *, void *ptr) {   \
			auto self                                              \
			    = euler::util::Reference<CLASS>::unwrap(ptr);      \
			self.decrement();                                      \
		},                                                             \
	}

/* Strictly data type, non-object */
#define MAKE_DATA_TYPE(CLASS)                                                  \
	mrb_data_type                                                          \
	{                                                                      \
		.struct_name = #CLASS, .dfree = [](mrb_state *, void *ptr) {   \
			auto self = static_cast<CLASS *>(ptr);                 \
			delete self;                                           \
		},                                                             \
	}

template <typename T>
static util::Reference<T>
unwrap(mrb_state *mrb, const mrb_value &value, const mrb_data_type *type)
{
	if (mrb_nil_p(value)) return nullptr;
	auto ptr = mrb_data_get_ptr(mrb, value, type);
	return util::Reference<T>::unwrap(ptr);
}

template <typename T>
static T *
unwrap_data(mrb_state *mrb, const mrb_value &value, const mrb_data_type *type)
{
	if (mrb_nil_p(value)) return nullptr;
	auto ptr = mrb_data_get_ptr(mrb, value, type);
	if (ptr != nullptr) return static_cast<T *>(ptr);
	mrb_raisef(mrb, E_TYPE_ERROR, "Expected a %s object",
	    type->struct_name);
	return nullptr;
}

#define ATTR_READER(SELF_TYPE, SELF_DATA, OTHER_DATA, SUPER, EXPR)             \
	[](mrb_state *mrb, const mrb_value self_value) {                       \
		const auto self                                                \
		    = unwrap_data<SELF_TYPE>(mrb, self_value, &SELF_DATA);     \
		const auto state = State::get(mrb);                            \
		const auto mod = state->module();                              \
		auto ans = (EXPR);                                             \
		const auto obj                                                 \
		    = Data_Wrap_Struct(mrb, (SUPER), &OTHER_DATA, ans.wrap()); \
		return mrb_obj_value(obj);                                     \
	}

#define ATTR_READER_DATA(SELF_TYPE, SELF_DATA, OTHER_TYPE, OTHER_DATA, EXPR)   \
	[](mrb_state *mrb, const mrb_value self_value) {                       \
		const auto self                                                \
		    = unwrap_data<SELF_TYPE>(mrb, self_value, &SELF_DATA);     \
		const auto ans = (EXPR);                                       \
		if (ans == nullptr) return mrb_nil_value();                    \
		const auto obj = Data_Wrap_Struct(mrb, mrb->object_class,      \
		    &OTHER_DATA, static_cast<OTHER_TYPE *>(ans));              \
		return mrb_obj_value(obj);                                     \
	}

#define ATTR_READER_VALUE(SELF_TYPE, SELF_DATA, WRAP, EXPR)                    \
	[](mrb_state *mrb, const mrb_value self_value) {                       \
		const auto self                                                \
		    = unwrap_data<SELF_TYPE>(mrb, self_value, &SELF_DATA);     \
		auto ans = (EXPR);                                             \
		return WRAP(ans);                                              \
	}

} /* namespace euler::app */

#endif /* EULER_APP_EXT_H */
