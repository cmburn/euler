/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_BUFFER_H
#define EULER_VULKAN_BUFFER_H

#include <vulkan/vulkan_raii.hpp>

#include "euler/util/object.h"

typedef struct VmaAllocation_T *VmaAllocation;

namespace euler::vulkan {
class Device;

class Buffer final : util::Object {
public:
	Buffer(util::Reference<Device> device, vk::DeviceSize buffer_size,
	    vk::BufferUsageFlags usage = vk::BufferUsageFlagBits::eTransferSrc,
	    vk::MemoryPropertyFlags properties
	    = vk::MemoryPropertyFlagBits::eHostVisible
		| vk::MemoryPropertyFlagBits::eHostCoherent);

	~Buffer() override;
	void load(const void *data, vk::DeviceSize size);
	void copy_to(util::Reference<Buffer> other) const;

private:
	util::Reference<Device> _device;
	VmaAllocation _allocation;
	vk::raii::Buffer _buffer;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_BUFFER_H */
