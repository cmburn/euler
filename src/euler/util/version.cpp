#include "version.h"

static constexpr euler::util::Version VERSION(0, 1, 0);

euler::util::Version
euler::util::version()
{
	return VERSION;
}

