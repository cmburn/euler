/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/descriptor_buffer.h"
#include "buffer.h"
#include "renderer.h"
#include "vk_mem_alloc.h"

euler::vulkan::DescriptorBuffer::DescriptorBuffer(util::Reference<Renderer> &r,
    vk::DeviceSize page_size)
    : _page_size(page_size)
    , _renderer(r)
{
	const auto limits = renderer()->physical_device().properties().limits;
	_alignment = std::max(limits.minStorageBufferOffsetAlignment,
	    limits.minUniformBufferOffsetAlignment);
}
void
euler::vulkan::DescriptorBuffer::begin_frame()
{
	for (auto &b : _internal_buffers) {
		const auto result = vmaMapMemory(renderer()->allocator(),
		    b.stage_buffer.allocation(), &b.host_data);
		if (result != VK_SUCCESS)
			throw std::runtime_error("Failed to map memory");
	}
}

void
euler::vulkan::DescriptorBuffer::end_frame(vk::raii::CommandBuffer &buf)
{
	for (auto &b : _internal_buffers) {
		vmaUnmapMemory(renderer()->allocator(),
		    b.stage_buffer.allocation());
		b.host_data = nullptr;
		if (b.size <= 0) continue;
		std::array<vk::BufferCopy, 1> regions = {};
		regions[0].size = (b.size < _page_size) ? b.size : _page_size;
		buf.copyBuffer(b.stage_buffer.buffer(),
		    b.device_buffer.buffer(), regions);
	}
}

euler::vulkan::DescriptorBuffer::BufferInfo
euler::vulkan::DescriptorBuffer::copy_data(const std::span<const uint8_t> data)
{
	auto *spot = find_buffer(data.size());
	const auto ptr = static_cast<uint8_t *>(spot->host_data) + spot->size;
	std::memcpy(ptr, data.data(), data.size());
	const auto info = BufferInfo {
		.buffer = &spot->device_buffer.buffer(),
		.offset = spot->size,
	};
	spot->update_size(data.size(), _alignment);
	return info;
}

euler::vulkan::DescriptorBuffer::BufferInfo
euler::vulkan::DescriptorBuffer::reserve_space(const vk::DeviceSize size)
{
	auto *spot = find_buffer(size);
	const auto info = BufferInfo {
		.buffer = &spot->device_buffer.buffer(),
		.offset = spot->size,
	};
	spot->update_size(size, _alignment);
	return info;
}

void
euler::vulkan::DescriptorBuffer::record_copy_pipeline_barrier(
    vk::raii::CommandBuffer &buf)
{
	record_barrier(vk::AccessFlagBits::eTransferWrite,
	    vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite,
	    renderer()->physical_device().graphics_family(),
	    vk::PipelineStageFlagBits::eTransfer,
	    vk::PipelineStageFlagBits::eComputeShader, buf);
}

void
euler::vulkan::DescriptorBuffer::record_compute_pipeline_barrier(
    vk::raii::CommandBuffer &buf)
{
#ifdef EULER_ENABLE_COMPUTE_QUEUE
	const auto qf = renderer()->physical_device().compute_family();
#else
	const auto qf = renderer()->physical_device().graphics_family();
#endif
	record_barrier(vk::AccessFlagBits::eShaderWrite,
	    vk::AccessFlagBits::eVertexAttributeRead, qf,
	    vk::PipelineStageFlagBits::eComputeShader,
	    vk::PipelineStageFlagBits::eVertexInput, buf);
}

euler::util::Reference<euler::vulkan::Renderer>
euler::vulkan::DescriptorBuffer::renderer() const
{
	return _renderer.strengthen();
}

void
euler::vulkan::DescriptorBuffer::InternalBuffer::update_size(
    const vk::DeviceSize data_size, const vk::DeviceSize alignment)
{
	if (data_size % alignment == 0) size += data_size;
	else size += ((data_size / alignment) + 1) * alignment;
}

euler::vulkan::DescriptorBuffer::InternalBuffer *
euler::vulkan::DescriptorBuffer::append_buffer()
{
	static const auto BARRIER = vk::BufferMemoryBarrier();
	const auto make_buffer
	    = [&](const vk::BufferUsageFlags usage,
		  const vk::MemoryPropertyFlags properties) -> Buffer {
		const auto r = renderer();
		return Buffer(r->device(), _page_size, usage, properties);
	};
	static constexpr vk::BufferUsageFlags DEVICE_FLAGS
	    = vk::BufferUsageFlagBits::eTransferDst
	    | vk::BufferUsageFlagBits::eUniformBuffer
	    | vk::BufferUsageFlagBits::eVertexBuffer
	    | vk::BufferUsageFlagBits::eStorageBuffer;

	InternalBuffer buf = {
		.device_buffer = make_buffer(DEVICE_FLAGS,
		    vk::MemoryPropertyFlagBits::eDeviceLocal),
		.stage_buffer
		= make_buffer(vk::BufferUsageFlagBits::eTransferSrc,
		    vk::MemoryPropertyFlagBits::eHostVisible),
		.host_data = nullptr,
		.size = 0,
	};
	_internal_buffers.emplace_back(std::move(buf));
	_memory_barriers.emplace_back(BARRIER);
	return &_internal_buffers.back();
}

euler::vulkan::DescriptorBuffer::InternalBuffer *
euler::vulkan::DescriptorBuffer::find_buffer(const vk::DeviceSize size)
{
	if (size >= _page_size) {
		throw std::runtime_error(
		    "Data size exceeds descriptor buffer page size");
	}
	InternalBuffer *spot = nullptr;
	for (auto &b : _internal_buffers) {
		if (size <= _page_size - b.size) {
			spot = &b;
			break;
		}
	}
	if (spot == nullptr) {
		spot = append_buffer();
		assert(spot != nullptr);
		spot->size = 0;
		const auto result = vmaMapMemory(renderer()->allocator(),
		    spot->stage_buffer.allocation(), &spot->host_data);
		if (result != VK_SUCCESS)
			throw std::runtime_error("Failed to map memory");
	}
	return spot;
}

void
euler::vulkan::DescriptorBuffer::record_barrier(
    const vk::AccessFlags src_access, const vk::AccessFlags dst_access,
    const uint32_t queue_family, const vk::PipelineStageFlags src_stage,
    const vk::PipelineStageFlags dst_stage, vk::raii::CommandBuffer &buf)
{
	assert(_memory_barriers.size() == _internal_buffers.size());
	auto barrier_count = 0;
	for (size_t i = 0; i < _memory_barriers.size(); ++i) {
		const auto &ib = _internal_buffers[i];
		if (ib.size <= 0) break;
		auto &barrier = _memory_barriers[i];
		barrier.srcAccessMask = src_access;
		barrier.dstAccessMask = dst_access;
		barrier.srcQueueFamilyIndex = queue_family;
		barrier.dstQueueFamilyIndex = queue_family;
		barrier.buffer = ib.device_buffer.buffer();
		barrier.offset = 0;
		barrier.size = ib.size;
		++barrier_count;
	}

	buf.pipelineBarrier(src_stage, dst_stage, vk::DependencyFlags(0),
	    nullptr,
	    { static_cast<uint32_t>(barrier_count), _memory_barriers.data() },
	    nullptr);
}