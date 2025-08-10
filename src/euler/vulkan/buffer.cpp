/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/buffer.h"

#include <vk_mem_alloc.h>

#include "euler/vulkan/device.h"
#include "euler/vulkan/renderer.h"

static vk::raii::Buffer
make_buffer(euler::util::Reference<euler::vulkan::Device> device,
    VmaAllocation *allocation, const vk::DeviceSize buffer_size,
    const vk::BufferUsageFlags usage, const vk::MemoryPropertyFlags properties)
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
    , _device(device)
    , _allocation(VK_NULL_HANDLE)
    , _buffer(make_buffer(device, &_allocation, buffer_size, usage, properties))
    , _buffer_size(buffer_size)
{
}

euler::vulkan::Buffer::~Buffer()
{
	const vk::Buffer vk_buffer = *_buffer;
	vmaDestroyBuffer(_device->renderer()->allocator(), vk_buffer,
	    _allocation);
}

euler::util::Reference<euler::vulkan::Buffer>
euler::vulkan::Buffer::load(util::Reference<Device> device,
    vk::BufferUsageFlags usage, std::vector<Data> data)
{
	vk::DeviceSize buffer_size = 0;
	for (const auto &[data, size] : data) buffer_size += size;
	const auto stage
	    = util::make_reference<Buffer>(device, buffer_size, usage);
	void *location = nullptr;
	vmaMapMemory(stage->_device->renderer()->allocator(),
	    stage->_allocation, &location);
	for (const auto &[data, size] : data) {
		if (data == nullptr || size == 0) continue;
		std::memcpy(location, data, size);
		location = static_cast<char *>(location) + size;
	}
	vmaUnmapMemory(stage->_device->renderer()->allocator(),
	    stage->_allocation);
	auto ret = util::make_reference<Buffer>(device, buffer_size, usage);
	stage->copy_to(ret);
	return ret;
}

euler::util::Reference<euler::vulkan::Buffer>
euler::vulkan::Buffer::load(util::Reference<Device> device,
    const vk::BufferUsageFlags usage, const Data data)
{
	return load(device, usage, std::vector { data });
}
void
euler::vulkan::Buffer::copy_to(util::Reference<Buffer> other) const
{
	auto command_buffer = _device->single_use_buffer();
	const vk::BufferCopy copy_region = {
		.srcOffset = static_cast<vk::DeviceSize>(0),
		.dstOffset = static_cast<vk::DeviceSize>(0),
		.size = _buffer_size,
	};
	command_buffer.copyBuffer(*_buffer, *other->_buffer, copy_region);
	_device->submit_single_use_buffer(command_buffer);
}