/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_DESCRIPTOR_BUFFER_H
#define EULER_VULKAN_DESCRIPTOR_BUFFER_H

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "buffer.h"
#include "euler/util/object.h"
#include "euler/vulkan/data.h"

namespace euler::vulkan {
class Renderer;

class DescriptorBuffer final : public util::Object {
public:
	// vk::DeviceSize copy_data(Data data, vk::raii::Buffer *dst);
	struct BufferInfo {
		vk::raii::Buffer &buffer;
		vk::DeviceSize offset;
	};

	void begin_frame(vk::raii::CommandBuffer &copy_buffer);
	void end_frame(vk::raii::CommandBuffer &copy_buffer);
	BufferInfo copy_data(Data data);
	BufferInfo reserve_space(vk::DeviceSize size);
	void record_copy_barrier(vk::raii::CommandBuffer &copy_buffer);
	void record_compute_barrier(vk::raii::CommandBuffer &copy_buffer);


private:
	vk::DeviceSize _page_size = 0;
	util::WeakReference<Renderer> _renderer;
	struct InternalBuffer {
		util::Reference<Buffer> device_buffer;
		util::Reference<Buffer> stage_buffer;
		Data data;
	};
	std::vector<InternalBuffer> _internal_buffers;
	vk::raii::CommandBuffer _copy_buffer;
	std::vector<vk::BufferMemoryBarrier> _memory_barriers;

};

} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_DESCRIPTOR_BUFFER_H */
