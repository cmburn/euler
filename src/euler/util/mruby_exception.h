/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_MRUBY_EXCEPTION_H
#define EULER_UTIL_MRUBY_EXCEPTION_H

#include <string>
#include <mruby.h>
#include <stdexcept>

#include "euler/util/object.h"

namespace euler::util {
class MRubyException final : public std::exception {
public:
	MRubyException(std::string message)
	    : _message(std::move(message))
	{
	}

	const char *what() const noexcept override
	{
		return _message.c_str();
	}

private:
	std::string _message;
};

} /* namespace euler::util */


#endif /* EULER_UTIL_MRUBY_EXCEPTION_H */

