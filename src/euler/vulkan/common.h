/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_COMMON_H
#define EULER_VULKAN_COMMON_H

#include <vector>
#include <cstdint>

namespace euler::vulkan {

enum class MSAA {
	x1 = 1,
	x2 = 2,
	x4 = 4,
	x8 = 8,
	x16 = 16,
	x32 = 32,
	x64 = 64,
};

} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_COMMON_H */

