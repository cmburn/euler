/* SPDX-License-Identifier: ISC */

#ifndef EULER_GAME_GUI_EXT_H
#define EULER_GAME_GUI_EXT_H

#include "euler/game/ext.h"
#include "euler/game/state.h"

namespace euler::game {

extern const mrb_data_type GUI_BUTTON_TYPE;

void init_gui(util::Reference<State> state);

} /* namespace euler::game */

#endif /* EULER_GAME_GUI_EXT_H */
