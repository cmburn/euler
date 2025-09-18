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
void
euler::vulkan::CommandBuffer::reset()
{
	/* no-opp */
}
void
euler::vulkan::CommandBuffer::begin(vk::CommandBufferUsageFlags flags,
    vk::CommandBufferInheritanceInfo *inheritance_info)
{
	(void)flags;
	(void)inheritance_info;
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
void
euler::vulkan::CommandBuffer::bind_pipeline(vk::PipelineBindPoint bind_point,
    util::Reference<Pipeline> pipeline)
{
	(void)bind_point;
	(void)pipeline;
}
void
euler::vulkan::CommandBuffer::begin_render_pass(
    const vk::RenderPassBeginInfo &rpbi, vk::SubpassContents contents)
{
	(void)rpbi;
	(void)contents;
}

void
euler::vulkan::CommandBuffer::end_render_pass()
{
}
void
euler::vulkan::CommandBuffer::end()
{
}
void
euler::vulkan::CommandBuffer::push_constants()
{
}
