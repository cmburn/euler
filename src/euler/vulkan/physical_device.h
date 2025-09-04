/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_PHYSICAL_DEVICE_H
#define EULER_VULKAN_PHYSICAL_DEVICE_H

#include <vulkan/vulkan_raii.hpp>

#include "euler/vulkan/common.h"

namespace euler::vulkan {
class Renderer;

class PhysicalDevice final {
public:
	PhysicalDevice(Renderer *renderer, vk::raii::PhysicalDevice &&pdev);

	const vk::raii::PhysicalDevice &
	physical_device() const
	{
		return _physical_device;
	}

	vk::raii::PhysicalDevice &
	physical_device()
	{
		return _physical_device;
	}

	uint32_t graphics_family() const
	{
		return _graphics_family;
	}

	uint32_t present_family() const
	{
		return _present_family;
	}

	uint32_t compute_family() const
	{
		return _compute_family;
	}

	MSAA msaa() const;

private:
	vk::raii::PhysicalDevice _physical_device;
	Renderer *_renderer;
	uint32_t _graphics_family;
	uint32_t _present_family;
	uint32_t _compute_family;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_PHYSICAL_DEVICE_H */
