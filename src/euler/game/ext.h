/* SPDX-License-Identifier: ISC */

#ifndef EULER_GAME_EXT_H
#define EULER_GAME_EXT_H

#include <mruby/data.h>
#include <mruby/presym.h>

/* common extension utilities */

namespace euler::game {

/* Standard Euler Object type */
#define REFERENCE_TYPE(CLASS)                                                  \
	mrb_data_type                                                          \
	{                                                                      \
		.struct_name = #CLASS, .dfree = [](mrb_state *, void *ptr) {   \
			auto self                                              \
			    = euler::util::Reference<CLASS>::unwrap(ptr);      \
			self.decrement();                                      \
		},                                                             \
	}

/* Strictly data type, non-object */
#define DATA_TYPE(CLASS)                                                       \
	mrb_data_type                                                          \
	{                                                                      \
		.struct_name = #CLASS, .dfree = [](mrb_state *, void *ptr) {   \
			auto self = static_cast<CLASS *>(ptr);                 \
			delete ptr;                                            \
		},                                                             \
	}

template <typename T>
static util::Reference<T>
unwrap(mrb_state *mrb, const mrb_value &value, const mrb_data_type *type)
{
	if (mrb_nil_p(value)) return nullptr;
	auto ptr = mrb_data_get_ptr(mrb, value, &REFERENCE_TYPE(T));
	return util::Reference<T>::unwrap(ptr);
}

// template <typename T>
// static T *
// unwrap_ptr(mrb_state *mrb, const mrb_value &value, const mrb_data_type *type)
// {
// 	if (mrb_nil_p(value)) return nullptr;
// 	auto ptr = mrb_data_get_ptr(mrb, value, type);
// 	if (ptr != nullptr) return static_cast<T *>(ptr);
// 	mrb_raisef(mrb, E_TYPE_ERROR, "Expected a %s object",
// 	    type->struct_name);
// }

} /* namespace euler::game */

#endif /* EULER_GAME_EXT_H */
