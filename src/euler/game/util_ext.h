/* SPDX-License-Identifier: ISC */

#ifndef EULER_GAME_UTIL_EXT_H
#define EULER_GAME_UTIL_EXT_H

#include "euler/game/state.h"
#include "euler/game/ext.h"

namespace euler::game {
static constexpr auto LOGGER_TYPE = MAKE_REFERENCE_TYPE(euler::util::Logger);
static constexpr auto LOGGER_SINK_TYPE
    = MAKE_DATA_TYPE(euler::util::Logger::Sink);
static constexpr auto STORAGE_TYPE = MAKE_REFERENCE_TYPE(euler::util::Storage);
static constexpr auto CONFIG_TYPE = MAKE_DATA_TYPE(euler::util::Config);
static constexpr auto VERSION_TYPE = MAKE_DATA_TYPE(euler::util::Version);

void init_util(util::Reference<State> state);
} /* namespace euler::game */


#endif /* EULER_GAME_UTIL_EXT_H */

