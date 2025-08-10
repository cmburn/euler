/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_BUFFER_H
#define EULER_VULKAN_BUFFER_H

#include <vulkan/vulkan_raii.hpp>

#include "euler/util/object.h"
#include "euler/vulkan/data.h"

typedef struct VmaAllocation_T *VmaAllocation;

namespace euler::vulkan {
class Device;

class Buffer final : public util::Object {
public:
	Buffer(util::Reference<Device> device, vk::DeviceSize buffer_size,
	    vk::BufferUsageFlags usage = vk::BufferUsageFlagBits::eTransferSrc,
	    vk::MemoryPropertyFlags properties
	    = vk::MemoryPropertyFlagBits::eHostVisible
		| vk::MemoryPropertyFlagBits::eHostCoherent);

	~Buffer() override;
	static util::Reference<Buffer> load(util::Reference<Device> device,
	    vk::BufferUsageFlags usage, std::vector<Data> data);

	static util::Reference<Buffer> load(util::Reference<Device> device,
	    vk::BufferUsageFlags usage, Data data);
	void copy_to(util::Reference<Buffer> other) const;

private:
	util::Reference<Device> _device;
	VmaAllocation _allocation;
	vk::raii::Buffer _buffer;
	vk::DeviceSize _buffer_size;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_BUFFER_H */
