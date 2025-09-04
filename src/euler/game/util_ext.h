/* SPDX-License-Identifier: ISC */

#ifndef EULER_GAME_UTIL_EXT_H
#define EULER_GAME_UTIL_EXT_H

#include "euler/game/state.h"
#include "euler/game/ext.h"

namespace euler::game {
extern const mrb_data_type LOGGER_TYPE;
extern const mrb_data_type LOGGER_SINK_TYPE;
extern const mrb_data_type STORAGE_TYPE;
extern const mrb_data_type CONFIG_TYPE;
extern const mrb_data_type VERSION_TYPE;

void init_util(util::Reference<State> state);
} /* namespace euler::game */


#endif /* EULER_GAME_UTIL_EXT_H */

