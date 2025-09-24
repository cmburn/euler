/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_DEVICE_H
#define EULER_VULKAN_DEVICE_H

#include <vulkan/vulkan_raii.hpp>

#include "euler/util/storage.h"
#include "euler/vulkan/physical_device.h"

namespace euler::vulkan {
class Renderer;
class Device;

class Device final {
public:
	Device(const util::Reference<Renderer> &renderer,
	    vk::raii::Device &&device, bool graphics_device = true);

	Device(std::nullptr_t = nullptr);

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

	const PhysicalDevice &
	physical_device() const
	{
		return *_physical_device;
	}

	PhysicalDevice &
	physical_device()
	{
		return *_physical_device;
	}

	vk::raii::Queue &
	queue()
	{
		return _queue;
	}

	const vk::raii::Queue &
	queue() const
	{
		return _queue;
	}

	util::Reference<Renderer> renderer() const;

	void reset_pool();
	void submit_single_use_buffer(vk::raii::CommandBuffer &cmd_buf) const;
	vk::raii::CommandBuffer command_buffer();
	vk::raii::CommandBuffers command_buffers(size_t n) const;
	vk::raii::CommandBuffer single_use_buffer();
	vk::raii::Fence fence(vk::FenceCreateFlags flags) const;
	vk::raii::Semaphore semaphore() const;

private:
	// void load_thread();

	vk::raii::Queue make_queue(bool graphics_device, uint32_t idx) const;
	vk::raii::CommandPool make_command_pool() const;

	vk::raii::Device _device;
	PhysicalDevice *_physical_device;
	vk::raii::Queue _queue;
	vk::raii::Queue _present_queue;
	// vk::raii::Queue _load_queue;
	vk::raii::CommandPool _pool;
	// vk::raii::CommandPool _load_pool;
	util::WeakReference<Renderer> _renderer;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_DEVICE_H */
