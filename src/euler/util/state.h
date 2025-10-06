/* SPDX-License-Identifier: ISC */

#ifndef EULER_MRUBY_STATE_H
#define EULER_MRUBY_STATE_H

#include <mutex>
#include <optional>

#include <mruby.h>

#include "euler/util/logger.h"
#include "euler/util/mruby_exception.h"
#include "euler/util/object.h"
#include "euler/util/thread.h"

namespace euler::util {
class Storage;

/* pure virtual class; the actual state is implemented in game::State */
class State : public Object {
protected:
	void init_fs(const char *argv0 = nullptr);
	void deinit_fs();

public:
	~State() override = default;
	[[nodiscard]] virtual MRubyException make_exception(RObject *exc) const
	    = 0;
	[[nodiscard]] virtual Reference<Logger> log() const = 0;
	[[nodiscard]] virtual Reference<Storage> user_storage() const = 0;
	[[nodiscard]] virtual Reference<Storage> title_storage() const = 0;
	[[nodiscard]] virtual std::unique_lock<std::mutex> lock_mrb() const = 0;
	[[nodiscard]] virtual std::optional<std::unique_lock<std::mutex>>
	try_lock_mrb() const = 0;
	[[nodiscard]] virtual mrb_state *mrb() const = 0;
	[[nodiscard]] virtual nthread_t available_threads() const = 0;
};
} /* namespace Euler::MRuby */

#endif /* EULER_MRUBY_STATE_H */
