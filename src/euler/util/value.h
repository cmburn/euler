/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_UTIL_VALUE_H
#define EULER_UTIL_VALUE_H

#include "euler/util/object.h"

namespace euler::util {
class Value final : public Object {
public:
	~Value() override;
	Value(Object *parent, mrb_value value);

	enum class Type {
		Bool,
		Undef,
		Nil,
		Float,
		Int,
		Symbol,
		String,
		Array,
		Hash,

	};

	mrb_value
	value() const
	{
		return _value;
	}

	void
	set_value(const mrb_value value)
	{
		_value = value;
	}

private:
	RClass;
	mrb_value _value = mrb_nil_value();
};
} /* namespace euler::util */

#endif /* EULER_UTIL_VALUE_H */
