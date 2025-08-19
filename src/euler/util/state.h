/* SPDX-License-Identifier: ISC */

#ifndef EULER_MRUBY_STATE_H
#define EULER_MRUBY_STATE_H

#include "euler/util/logger.h"
#include "euler/util/object.h"

namespace euler::util {
class Storage;



/* pure virtual class; the actual state is implemented in game::State */
class State : public Object {
protected:
	void init_fs(const char *argv0 = nullptr);
	void deinit_fs();

public:
	~State() override = default;
	[[nodiscard]] virtual Reference<Logger> log() const = 0;
	[[nodiscard]] virtual Reference<Storage> storage() const = 0;
};
} /* namespace Euler::MRuby */

#endif /* EULER_MRUBY_STATE_H */
