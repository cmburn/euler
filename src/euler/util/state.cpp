/* SPDX-License-Identifier: ISC */

#include "euler/util/state.h"

#include <physfs.h>

void
euler::util::State::init_fs(const char *argv0)
{
	PHYSFS_init(argv0);
}

void
euler::util::State::deinit_fs()
{
	PHYSFS_deinit();
}