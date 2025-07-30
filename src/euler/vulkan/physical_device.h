/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_PHYSICAL_DEVICE_H
#define EULER_VULKAN_PHYSICAL_DEVICE_H

#include "euler/util/object.h"

namespace euler::vulkan {
class PhysicalDevice final : util::Object {
public:
	uint32_t graphics_family() const;
	uint32_t compute_family() const;
};
} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_PHYSICAL_DEVICE_H */

