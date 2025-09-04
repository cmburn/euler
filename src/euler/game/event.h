/* SPDX-License-Identifier: ISC */

#ifndef EULER_GAME_GAME_EVENT_EXT_H
#define EULER_GAME_GAME_EVENT_EXT_H

#include "euler/game/ext.h"
#include "euler/game/state.h"

namespace euler::game {

void init_game_event(util::Reference<State> state);
mrb_value sdl_event_to_mrb(util::Reference<State> state,
    const SDL_Event &event);

} /* namespace euler::game */

#endif /* EULER_GAME_GAME_EVENT_EXT_H */
