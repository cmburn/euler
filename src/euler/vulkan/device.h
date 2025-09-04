/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_DEVICE_H
#define EULER_VULKAN_DEVICE_H

#include <vulkan/vulkan_raii.hpp>

#include "euler/util/storage.h"
#include "euler/vulkan/physical_device.h"

namespace euler::vulkan {
class Renderer;

class Device final {
public:
	Device(Renderer *renderer, vk::raii::Device &&device);

	const vk::raii::Device &
	device() const
	{
		return _device;
	}

	vk::raii::Device &
	device()
	{
		return _device;
	}

	PhysicalDevice &
	physical_device() const
	{
		return _physical_device;
	}

	util::Reference<Renderer> renderer() const;

	void reset_pool();
	void submit_single_use_buffer(vk::raii::CommandBuffer &cmd_buf) const;
	vk::raii::CommandBuffer command_buffer();
	vk::raii::CommandBuffers command_buffers(size_t n);
	vk::raii::CommandBuffer single_use_buffer();
	vk::raii::Fence fence(vk::FenceCreateFlags flags) const;
	vk::raii::Semaphore semaphore() const;

private:
	vk::raii::Device _device;
	vk::raii::CommandPool _pool;
	vk::raii::CommandPool _load_pool;
	vk::raii::Queue _queue;
	vk::raii::Queue _load_queue;
	util::WeakReference<Renderer> _renderer;
	PhysicalDevice &_physical_device;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_DEVICE_H */
