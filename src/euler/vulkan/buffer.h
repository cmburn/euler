/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_BUFFER_H
#define EULER_VULKAN_BUFFER_H

#include <vulkan/vulkan_raii.hpp>
#include <vk_mem_alloc.h>

namespace euler::vulkan {
class Device;

class Buffer final {
public:
	Buffer(Device &device, vk::DeviceSize buffer_size,
	    vk::BufferUsageFlags usage = vk::BufferUsageFlagBits::eTransferSrc,
	    vk::MemoryPropertyFlags properties
	    = vk::MemoryPropertyFlagBits::eHostVisible
		| vk::MemoryPropertyFlagBits::eHostCoherent);

	Buffer(Buffer &&) = default;
	~Buffer();
	static Buffer load(Device &device,
	    vk::BufferUsageFlags usage,
	    const std::vector<std::span<const uint8_t>> &data);


	static Buffer load(Device &device,
	    vk::BufferUsageFlags usage, std::span<const uint8_t> data);
	void copy_to(Buffer &other) const;

	VmaAllocation &allocation() { return _allocation; }
	const VmaAllocation &allocation() const { return _allocation; }

private:
	Device &_device;
	VmaAllocation _allocation;
	vk::raii::Buffer _buffer;
	vk::DeviceSize _buffer_size;
};
} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_BUFFER_H */

