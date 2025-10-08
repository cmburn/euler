/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_GAME_EVENT_EXT_H
#define EULER_APP_GAME_EVENT_EXT_H

#include "euler/app/ext.h"
#include "euler/app/state.h"

namespace euler::app {

extern const mrb_data_type EVENT_TYPE;

void init_app_event(util::Reference<State> state);
mrb_value sdl_event_to_mrb(util::Reference<State> state,
    const SDL_Event &event);

} /* namespace euler::app */

#endif /* EULER_APP_GAME_EVENT_EXT_H */
