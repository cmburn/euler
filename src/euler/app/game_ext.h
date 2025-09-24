/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_GAME_EXT_H
#define EULER_APP_GAME_EXT_H

#include "euler/app/ext.h"
#include "euler/app/state.h"

namespace euler::app {

extern const mrb_data_type STATE_TYPE;
extern const mrb_data_type SYSTEM_TYPE;

void init_game(util::Reference<State> state);

} /* namespace euler::app */


#endif /* EULER_APP_GAME_EXT_H */

