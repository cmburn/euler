/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_THREAD_H
#define EULER_UTIL_THREAD_H

#include <functional>

namespace euler::util {

using Callback = std::function<void()>;

bool is_main_thread();
/* SHOULD ONLY BE CALLED AFTER SDL IS INITIALIZED */
bool run_on_main_thread(const Callback &fn, bool wait_complete = true);

} /* namespace Euler::Util */

#endif /* EULER_UTIL_THREAD_H */

