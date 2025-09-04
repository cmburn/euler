/* SPDX-License-Identifier: ISC */

#ifndef EULER_GAME_GRAPHICS_EXT_H
#define EULER_GAME_GRAPHICS_EXT_H

#include "euler/game/state.h"
#include "euler/game/ext.h"

namespace euler::game {

extern const mrb_data_type GRAPHICS_COLOR_TYPE;

void init_graphics(util::Reference<State> state);

} /* namespace euler::game */


#endif /* EULER_GAME_GRAPHICS_EXT_H */

