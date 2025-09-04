/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/descriptor_buffer.h"
#include "renderer.h"
#include "vk_mem_alloc.h"

void
euler::vulkan::DescriptorBuffer::begin_frame()
{
	for (auto &b : _internal_buffers) {

	}
}
void
euler::vulkan::DescriptorBuffer::end_frame(vk::raii::CommandBuffer &copy_buffer)
{
}