/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_THREAD_H
#define EULER_UTIL_THREAD_H

#include <functional>
#include <thread>

namespace euler::util {

using nthread_t = decltype(std::thread::hardware_concurrency());

bool is_main_thread();
void run_on_main_thread(const std::function<void()> &fn, bool wait = false);

} /* namespace euler::util */


#endif /* EULER_UTIL_THREAD_H */

