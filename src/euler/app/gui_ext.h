/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_GUI_EXT_H
#define EULER_APP_GUI_EXT_H

#include "euler/app/ext.h"
#include "euler/app/state.h"

namespace euler::app {

extern const mrb_data_type GUI_BUTTON_TYPE;

void init_gui(util::Reference<State> state);

} /* namespace euler::app */

#endif /* EULER_APP_GUI_EXT_H */
