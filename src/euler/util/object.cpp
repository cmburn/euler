/* SPDX-License-Identifier: ISC */

#include "euler/util/object.h"

euler::util::Reference<euler::util::State>
euler::util::Object::state() const
{
	return _state.strengthen();
}