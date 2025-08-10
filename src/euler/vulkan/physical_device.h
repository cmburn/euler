/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_PHYSICAL_DEVICE_H
#define EULER_VULKAN_PHYSICAL_DEVICE_H

#include <vulkan/vulkan_raii.hpp>

#include "euler/util/object.h"
#include "euler/vulkan/instance.h"
#include "euler/vulkan/types.h"

namespace euler::vulkan {


class PhysicalDevice final : public util::Object {
public:
	PhysicalDevice(util::Reference<Instance> instance);

	uint32_t graphics_family() const;
	uint32_t compute_family() const;
	const vk::raii::PhysicalDevice &physical_device() const;
	vk::raii::PhysicalDevice &physical_device();
	MSAA msaa() const;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_PHYSICAL_DEVICE_H */
