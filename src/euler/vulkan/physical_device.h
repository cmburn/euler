/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_PHYSICAL_DEVICE_H
#define EULER_VULKAN_PHYSICAL_DEVICE_H

#include <vulkan/vulkan_raii.hpp>

#include "euler/util/object.h"

namespace euler::vulkan {
class Renderer;
class Surface;

class PhysicalDevice final {
public:
	PhysicalDevice(Renderer *renderer, vk::raii::PhysicalDevice &&pdev);
	PhysicalDevice(nullptr_t = nullptr);

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

	// uint32_t present_family() const
	// {
	// 	return _present_family;
	// }

	uint32_t compute_family() const
	{
		return _compute_family;
	}

	vk::SampleCountFlagBits msaa() const;

	vk::PhysicalDeviceProperties properties() const
	{
		return _physical_device.getProperties();
	}

	bool supports_surface(const util::Reference<Surface> &surface) const;
	static constexpr auto NO_QUEUE = std::numeric_limits<uint32_t>::max();
		void select_queue_families();


private:

	vk::raii::PhysicalDevice _physical_device;
	Renderer *_renderer;
	uint32_t _graphics_family = NO_QUEUE;
	// uint32_t _present_family = 0;
	uint32_t _compute_family = NO_QUEUE;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_PHYSICAL_DEVICE_H */
