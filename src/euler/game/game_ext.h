/* SPDX-License-Identifier: ISC */

#ifndef EULER_GAME_GAME_EXT_H
#define EULER_GAME_GAME_EXT_H

#include "euler/game/state.h"
#include "euler/game/ext.h"

namespace euler::game {

extern const mrb_data_type STATE_TYPE;

void init_game(util::Reference<State> state);

} /* namespace euler::game */


#endif /* EULER_GAME_GAME_EXT_H */

