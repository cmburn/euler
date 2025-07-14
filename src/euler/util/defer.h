/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_DEFER_H
#define EULER_UTIL_DEFER_H

#include <format>
#include <functional>
#include <iostream>

namespace euler::util {

template <typename... Args> struct Defer {
	std::function<void(Args...)> fn;
	explicit Defer(std::function<void(Args...)> fn)
	    : fn(fn)
	{
	}

	~Defer()
	{
		try {
			if (fn) fn();
		} catch (std::exception &e) {
			std::println(std::cerr,
			    "FATAL: Exception thrown in deferred function: {}",
			    e.what());
			abort();
		}
	}
};

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define DEFER(FN) ::euler::util::Defer CONCAT(_defer_, __LINE__)(FN)

} /* namespace Euler::Util */

#endif /* EULER_UTIL_DEFER_H */
