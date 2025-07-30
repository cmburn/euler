/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/buffer.h"

#include <vk_mem_alloc.h>

#include "euler/vulkan/device.h"
#include "euler/vulkan/renderer.h"

static vk::raii::Buffer
make_buffer(euler::util::Reference<euler::vulkan::Device> device,
    VmaAllocation *allocation,
    const vk::DeviceSize buffer_size, const vk::BufferUsageFlags usage,
    const vk::MemoryPropertyFlags properties)
{
	auto queue_family_index = device->physical_device()->graphics_family();
	const auto r = device->renderer();
	const VkBufferCreateInfo buffer_info = {
		.size = buffer_size,
		.usage = static_cast<VkBufferUsageFlags>(usage),
		.sharingMode
		= static_cast<VkSharingMode>(vk::SharingMode::eExclusive),
		.queueFamilyIndexCount = 1,
		.pQueueFamilyIndices = &queue_family_index,
	};
	const VmaAllocationCreateInfo allocation_info = {
		.requiredFlags = static_cast<VkMemoryPropertyFlags>(properties),
	};
	VkBuffer buffer = VK_NULL_HANDLE;

	const auto result = vmaCreateBuffer(r->allocator(), &buffer_info,
	    &allocation_info, &buffer, allocation, VK_NULL_HANDLE);
	if (result != VK_SUCCESS)
		throw std::runtime_error("failed to create buffer");
	return vk::raii::Buffer(device->device(), buffer);
}

euler::vulkan::Buffer::Buffer(util::Reference<Device> device,
    const vk::DeviceSize buffer_size, const vk::BufferUsageFlags usage,
    const vk::MemoryPropertyFlags properties)
    : Object(device->state())
    , _device(std::move(device))
    , _allocation(VK_NULL_HANDLE)
    , _buffer(make_buffer(device, &_allocation, buffer_size, usage, properties))
{
}

void
euler::vulkan::Buffer::load(const void *data, vk::DeviceSize size)
{
}

void
euler::vulkan::Buffer::copy_to(util::Reference<Buffer> other) const
{

}