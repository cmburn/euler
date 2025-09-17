/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_COMMAND_BUFFER_H
#define EULER_VULKAN_COMMAND_BUFFER_H

#include <thread>

#include <vulkan/vulkan_raii.hpp>

#include "euler/util/object.h"

namespace euler::vulkan {
class Pipeline;

class CommandBuffer final {
public:
	CommandBuffer();

	void push_descriptor_set(const util::Reference<Pipeline> &pipeline,
	    std::span<const vk::WriteDescriptorSet> writes,
	    vk::PipelineBindPoint bind_point = vk::PipelineBindPoint::eCompute);
	void reset();
	void begin(vk::CommandBufferUsageFlags flags = {},
	    vk::CommandBufferInheritanceInfo *inheritance_info = nullptr);

	vk::raii::CommandBuffer &command_buffer();
	const vk::raii::CommandBuffer &command_buffer() const;

	void bind_pipeline(vk::PipelineBindPoint bind_point,
	    util::Reference<Pipeline> pipeline);
	void begin_render_pass(const vk::RenderPassBeginInfo &rpbi,
	    vk::SubpassContents contents = vk::SubpassContents::eInline);
	void end_render_pass();
	void end();
	void push_constants();

private:
	vk::raii::CommandBuffer _command_buffer;
	std::thread::id _thread_id;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_COMMAND_BUFFER_H */
