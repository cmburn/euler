/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_DEFER_H
#define EULER_UTIL_DEFER_H
#include <functional>

namespace Euler::Util {

template <typename T, typename... Args> struct Defer {
	std::function<T(Args...)> fn;
	Defer(std::function<T(Args...)> fn)
	    : fn(fn)
	{
	}

	~Defer()
	{
		if (fn) fn();
	}
};

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define DEFER(FN) ::Euler::Util::Defer<void> CONCAT(_defer_, __LINE__)(FN)

} /* namespace Euler::Util */

#endif /* EULER_UTIL_DEFER_H */
