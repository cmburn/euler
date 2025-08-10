/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_DATA_H
#define EULER_VULKAN_DATA_H

#include <vulkan/vulkan.hpp>

namespace euler::vulkan {
struct Data {
	vk::DeviceSize size;
	void *data;
};
} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_DATA_H */

