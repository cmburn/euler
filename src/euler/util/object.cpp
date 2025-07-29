/* SPDX-License-Identifier: ISC */

#include "euler/util/object.h"

#include "euler/util/state.h"

euler::util::Reference<euler::util::State>
euler::util::Object::state() const
{
	return _state.strengthen();
}

euler::util::Reference<euler::util::Logger>
euler::util::Object::log() const
{
	return state()->log();
}