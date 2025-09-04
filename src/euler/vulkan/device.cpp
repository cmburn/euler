/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/device.h"

#include "euler/vulkan/renderer.h"

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
	if (renderer()->on_render_thread()) {
		_queue.submit(submit_info, nullptr);
		_queue.waitIdle();
	} else {
		_load_queue.submit(submit_info, nullptr);
		_load_queue.waitIdle();
	}
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
euler::vulkan::Device::command_buffers(const size_t n)
{
	const vk::CommandBufferAllocateInfo alloc_info {
		.commandPool = _pool,
		.level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = static_cast<uint32_t>(n),
	};
	return _device.allocateCommandBuffers(alloc_info);
}

vk::raii::CommandBuffer
euler::vulkan::Device::single_use_buffer()
{
	auto cmd_buf = command_buffer();
	cmd_buf.begin({
	    .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
	});
	return std::move(cmd_buf);
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