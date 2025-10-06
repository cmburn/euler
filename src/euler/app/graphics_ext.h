/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_GRAPHICS_EXT_H
#define EULER_APP_GRAPHICS_EXT_H

#include "euler/app/ext.h"
#include "euler/app/state.h"

namespace euler::app {

extern const mrb_data_type GRAPHICS_COLOR_TYPE;

void init_graphics(util::Reference<State> state);

} /* namespace euler::app */


#endif /* EULER_APP_GRAPHICS_EXT_H */

