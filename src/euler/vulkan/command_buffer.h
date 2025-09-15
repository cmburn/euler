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

	void push_descriptor_set(util::Reference<Pipeline> pipeline,
	    std::span<const vk::WriteDescriptorSet> writes);
	void reset();
	void begin(vk::CommandBufferUsageFlags flags = {},
	    vk::CommandBufferInheritanceInfo *inheritance_info = nullptr);

private:
	vk::raii::CommandBuffer _command_buffer;
	std::thread::id _thread_id;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_COMMAND_BUFFER_H */
