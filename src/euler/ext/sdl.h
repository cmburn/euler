/* SPDX-License-Identifier: ISC */

#ifndef EULER_EXT_SDL_H
#define EULER_EXT_SDL_H

#include <mruby.h>
#include <SDL3/SDL_events.h>

namespace euler::ext {

mrb_value to_mrb_value(mrb_state *mrb, const SDL_Event &e);

} /* namespace euler::ext */

#endif /* EULER_EXT_SDL_H */
