/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/command_buffer.h"
#include "pipeline.h"

euler::vulkan::CommandBuffer::CommandBuffer()
    : _command_buffer(nullptr)
{
}

void
euler::vulkan::CommandBuffer::push_descriptor_set(
    const util::Reference<Pipeline> &pipeline,
    const std::span<const vk::WriteDescriptorSet> writes,
    const vk::PipelineBindPoint bind_point)
{
	command_buffer().pushDescriptorSetKHR(bind_point, pipeline->layout(), 0,
	    writes);
}

vk::raii::CommandBuffer &
euler::vulkan::CommandBuffer::command_buffer()
{
	if (_thread_id != std::this_thread::get_id()) {
		throw std::runtime_error(
		    "CommandBuffer used from different thread");
	}
	return _command_buffer;
}

const vk::raii::CommandBuffer &
euler::vulkan::CommandBuffer::command_buffer() const
{
	if (_thread_id != std::this_thread::get_id()) {
		throw std::runtime_error(
		    "CommandBuffer used from different thread");
	}
	return _command_buffer;
}
