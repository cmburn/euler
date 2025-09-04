/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_DESCRIPTOR_BUFFER_H
#define EULER_VULKAN_DESCRIPTOR_BUFFER_H

#include <span>
#include <vector>
#include <vulkan/vulkan_raii.hpp>
// #include <vk_mem_alloc.h>

#include "euler/util/object.h"
#include "euler/vulkan/common.h"

namespace euler::vulkan {
class Renderer;
class Buffer;

class DescriptorBuffer final {
public:
	DescriptorBuffer(util::Reference<Renderer> &, vk::DeviceSize page_size = 65536);
	// vk::DeviceSize copy_data(Data data, vk::raii::Buffer *dst);
	struct BufferInfo {
		vk::raii::Buffer &buffer;
		vk::DeviceSize offset;
	};

	void begin_frame();
	void end_frame(vk::raii::CommandBuffer &buf);
	BufferInfo copy_data(std::span<uint8_t> data);
	BufferInfo reserve_space(vk::DeviceSize size);
	void record_copy_pipeline_barrier(vk::raii::CommandBuffer &buf);
	void record_compute_pipeline_barrier(vk::raii::CommandBuffer &buf);
	util::Reference<Renderer> renderer() const;

private:
	vk::DeviceSize _page_size = 0;
	util::WeakReference<Renderer> _renderer;
	struct InternalBuffer {
		Buffer &device_buffer;
		Buffer &stage_buffer;
		std::vector<uint8_t> data;
	};
	std::vector<InternalBuffer> _internal_buffers;
	// vk::raii::CommandBuffer _copy_buffer;
	std::vector<vk::BufferMemoryBarrier> _memory_barriers;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_DESCRIPTOR_BUFFER_H */
