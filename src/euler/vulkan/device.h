/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_DEVICE_H
#define EULER_VULKAN_DEVICE_H

#include <vulkan/vulkan_raii.hpp>

#include "euler/util/object.h"
#include "euler/vulkan/physical_device.h"

namespace euler::vulkan {
class Renderer;
class Buffer;

class Device final : public util::Object {
public:
	util::Reference<Renderer> renderer() const;
	util::Reference<PhysicalDevice> physical_device() const;
	vk::raii::Device &device() { return _device; }
	const vk::raii::Device &device() const { return _device; }

	void reset_pool();
	void submit_single_use_buffer(
		vk::raii::CommandBuffer &command_buffer) const;
	vk::raii::CommandBuffer command_buffer();
	vk::raii::CommandBuffers command_buffers(size_t n);
	vk::raii::CommandBuffer single_use_buffer();
	vk::raii::Fence fence(vk::FenceCreateFlags) const;
	vk::raii::Semaphore semaphore() const;

private:
	vk::raii::Device _device;
};
} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_DEVICE_H */

