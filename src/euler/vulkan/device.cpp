/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/device.h"

#include "euler/vulkan/renderer.h"

euler::vulkan::Device::Device(const util::Reference<Renderer> &renderer,
    vk::raii::Device &&device, const bool graphics_device)
    : _device(std::move(device))
    , _physical_device(renderer->physical_device())
    , _queue(make_queue(graphics_device, 0))
    , _pool(make_command_pool())
    , _renderer(renderer)
{
}

euler::util::Reference<euler::vulkan::Renderer>
euler::vulkan::Device::renderer() const
{
	return _renderer.strengthen();
}

void
euler::vulkan::Device::reset_pool()
{
	_pool.reset();
}

void
euler::vulkan::Device::submit_single_use_buffer(
    vk::raii::CommandBuffer &cmd_buf) const
{
	vk::CommandBuffer cmd = static_cast<VkCommandBuffer>(*cmd_buf);
	const vk::SubmitInfo submit_info {
		.waitSemaphoreCount = 0,
		.pWaitSemaphores = nullptr,
		.pWaitDstStageMask = nullptr,
		.commandBufferCount = 1,
		.pCommandBuffers = &cmd,
		.signalSemaphoreCount = 0,
		.pSignalSemaphores = nullptr,
	};
	_queue.submit(submit_info, nullptr);
	_queue.waitIdle();
}

vk::raii::CommandBuffer
euler::vulkan::Device::command_buffer()
{
	const vk::CommandBufferAllocateInfo alloc_info {
		.commandPool = _pool,
		.level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = 1,
	};
	return std::move(_device.allocateCommandBuffers(alloc_info).front());
}

vk::raii::CommandBuffers
euler::vulkan::Device::command_buffers(const size_t n) const
{
	const vk::CommandBufferAllocateInfo alloc_info {
		.commandPool = _pool,
		.level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = static_cast<uint32_t>(n),
	};
	return vk::raii::CommandBuffers(_device, alloc_info);
}

vk::raii::CommandBuffer
euler::vulkan::Device::single_use_buffer()
{
	auto cmd_buf = command_buffer();
	cmd_buf.begin({
	    .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
	});
	return cmd_buf;
}

vk::raii::Fence
euler::vulkan::Device::fence(const vk::FenceCreateFlags flags) const
{
	const vk::FenceCreateInfo create_info {
		.flags = flags,
	};
	return _device.createFence(create_info);
}

vk::raii::Semaphore
euler::vulkan::Device::semaphore() const
{
	static constexpr vk::SemaphoreCreateInfo create_info = {};
	return _device.createSemaphore(create_info);
}

vk::raii::Queue
euler::vulkan::Device::make_queue(const bool graphics_device,
    const uint32_t idx) const
{
	const uint32_t qf = graphics_device ? _physical_device.graphics_family()
					    : _physical_device.compute_family();
	return _device.getQueue(qf, idx);
}

vk::raii::CommandPool
euler::vulkan::Device::make_command_pool() const
{
	const vk::CommandPoolCreateInfo create_info {
		.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
		.queueFamilyIndex = _physical_device.graphics_family(),
	};
	return _device.createCommandPool(create_info);
}
