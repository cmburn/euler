/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_UTIL_EXT_H
#define EULER_APP_UTIL_EXT_H

#include "euler/app/ext.h"
#include "euler/app/state.h"

namespace euler::app {
extern const mrb_data_type LOGGER_TYPE;
extern const mrb_data_type LOGGER_SINK_TYPE;
extern const mrb_data_type STORAGE_TYPE;
extern const mrb_data_type CONFIG_TYPE;
extern const mrb_data_type VERSION_TYPE;

void init_util(util::Reference<State> state);
} /* namespace euler::app */


#endif /* EULER_APP_UTIL_EXT_H */

