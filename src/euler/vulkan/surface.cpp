/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/surface.h"

#include "euler/vulkan/renderer.h"

euler::vulkan::Surface::Surface()
    : _surface(nullptr)
    , _swapchain(util::Reference(this))
{
}

void
euler::vulkan::Surface::initialize_vulkan(
    const util::Reference<Renderer> &renderer)
{
	_renderer = renderer.weaken();
}

void
euler::vulkan::Surface::start_frame(const util::Color clear)
{
	assert(_renderer != nullptr);
	auto &cf = _swapchain._frames[_current_frame];
	auto &dev = renderer()->device().device();
	if (dev.waitForFences({ 1, &*cf.in_flight }, true, UINT64_MAX)
	    != vk::Result::eSuccess) {
		renderer()->log()->fatal("Failed to wait for image fence");
	}
	const vk::AcquireNextImageInfoKHR ani_info {
		.swapchain = *_swapchain._swapchain,
		.timeout = UINT64_MAX,
		.semaphore = *cf.image_ready,
		.fence = nullptr,
		.deviceMask = _image_index,
	};

	if (const auto [result, index] = dev.acquireNextImage2KHR(ani_info);
	    result == vk::Result::eSuccess) {
		_image_index = index;
	} else {
		renderer()->log()->fatal("Failed to acquire next image");
	}
	auto &si = _swapchain._images[_image_index];
	if (si.image_in_flight != nullptr) {
		const auto &iif = *si.image_in_flight;
		if (dev.waitForFences({ 1, &*iif }, true, UINT64_MAX)
		    != vk::Result::eSuccess) {
			renderer()->log()->fatal(
			    "Failed to wait for image fence");
		}
	}
	si.image_in_flight = &cf.in_flight;
	vmaSetCurrentFrameIndex(renderer()->allocator(), _current_frame);
	// rt.subpass = 0;
	// rt.frame = &si.framebuffer;
	// rt.render_pass = &_swapchain._render_pass;
	// rt.image = &si.image;
	// rt.ubo_set = &cf.ubo_descriptor_set;
	// rt.target = nullptr; /* screen */

	si.command_buffer.reset();
	si.db_command_buffer.reset();
	si.compute_command_buffer.reset();

	si.command_buffer.begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
	si.db_command_buffer.begin(
	    vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
	si.compute_command_buffer.begin(
	    vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

	cf.descriptor_buffer.begin_frame();

	for (auto &ci : _cameras) ci.camera->update_ubo(ci.projection);
	flush_ubo_buffers(cf);
	const vk::Rect2D rect = {
		.offset = { .x = 0, .y = 0 },
		.extent = { .width = width(), .height = height() },
	};
	const std::array<uint32_t, 4> clr = {
		clear.red(),
		clear.green(),
		clear.blue(),
		clear.alpha(),
	};
	const std::array clear_values = {
		vk::ClearValue {
		    vk::ClearColorValue(clr),
		},
		vk::ClearValue {
		    vk::ClearDepthStencilValue { .depth = 1.0f, .stencil = 0 },
		},
	};
	const vk::RenderPassBeginInfo render_pass_info {
		.renderPass = *_swapchain._render_pass,
		.framebuffer = *si.framebuffer,
		.renderArea = rect,
		.clearValueCount = static_cast<uint32_t>(clear_values.size()),
		.pClearValues = clear_values.data(),
	};
	si.command_buffer.beginRenderPass(render_pass_info,
	    vk::SubpassContents::eInline);
	si.compute_command_buffer.bindPipeline(vk::PipelineBindPoint::eCompute,
	    _sprite_batch_pipe->compute_pipeline());
}

void
euler::vulkan::Surface::end_frame()
{
	flush_sprite_batch();

	auto &cf = _swapchain._frames[_current_frame];
	auto &ldev = renderer()->device();
	auto &dev = ldev.device();
	auto &si = _swapchain._images[_image_index];
	si.command_buffer.endRenderPass();
	cf.descriptor_buffer.end_frame(si.db_command_buffer);
	cf.descriptor_buffer.record_compute_pipeline_barrier(
	    si.db_command_buffer);
	cf.descriptor_buffer.record_compute_pipeline_barrier(
	    si.compute_command_buffer);
	si.command_buffer.end();
	si.db_command_buffer.end();
	si.compute_command_buffer.end();
	dev.resetFences({ 1, &*cf.in_flight });
	std::array cmd_buffers = {
		*si.db_command_buffer,
		*si.compute_command_buffer,
		*si.command_buffer,
	};
	static constexpr std::array<vk::PipelineStageFlags, 1> WAIT_STAGES = {
		vk::PipelineStageFlagBits::eColorAttachmentOutput,
	};
	vk::SubmitInfo submit_info {
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &*cf.image_ready,
		.pWaitDstStageMask = WAIT_STAGES.data(),
		.commandBufferCount = static_cast<uint32_t>(cmd_buffers.size()),
		.pCommandBuffers = cmd_buffers.data(),
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &*cf.render_complete,
	};
	renderer()->device().queue().submit({ 1, &submit_info }, *cf.in_flight);
	auto present_wait = gui_render();
	vk::Result present_result;
	vk::PresentInfoKHR present_info {
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &*present_wait,
		.swapchainCount = 1,
		.pSwapchains = &*_swapchain._swapchain,
		.pImageIndices = &_image_index,
		.pResults = &present_result,
	};
	bool rebuilt_swapchain = false;
	const auto queue_result = ldev.queue().presentKHR(present_info);
	switch (present_result) {
	case vk::Result::eSuccess: break;
	case vk::Result::eErrorOutOfDateKHR:
	case vk::Result::eSuboptimalKHR:
		_swapchain.rebuild();
		rebuilt_swapchain = true;
		break;
	default: throw std::runtime_error("Failed to present swapchain image");
	}
	switch (queue_result) {
	case vk::Result::eSuccess: break;
	case vk::Result::eErrorOutOfDateKHR:
	case vk::Result::eSuboptimalKHR:
		if (!rebuilt_swapchain) {
			_swapchain.rebuild();
			rebuilt_swapchain = true;
		}
		break;
	default: throw std::runtime_error("Failed to present swapchain image");
	}
	increment_current_frame();
}

void
euler::vulkan::Surface::flush_sprite_batch()
{
	/* TODO: PushDescriptorSet extension */
	auto &cf = _swapchain._frames[_current_frame];
	auto &si = _swapchain._images[_image_index];
	auto &db = cf.descriptor_buffer;
	/* TODO: Handle via Vulkan extension */
	const auto draw_command_data
	    = static_cast<void *>(_draw_commands.data());
	const auto cmd_size = sizeof(decltype(_draw_commands)::value_type)
	    * _draw_commands.size();
	auto [cmd_inst, cmd_offset] = db.copy_data({
	    static_cast<const uint8_t *>(draw_command_data),
	    cmd_size,
	});
	const auto draw_instance_size
	    = sizeof(decltype(_draw_instances)::value_type)
	    * _draw_instances.size();
	auto [draw_inst, draw_offset] = db.reserve_space(cmd_size);
	// const std::array buffer_info = {
	// 	vk::DescriptorBufferInfo {
	// 	    .buffer = *cmd_inst,
	// 	    .offset = cmd_offset,
	// 	    .range = cmd_size,
	// 	},
	// 	vk::DescriptorBufferInfo {
	// 	    .buffer = *draw_inst,
	// 	    .offset = draw_offset,
	// 	    .range = draw_instance_size,
	// 	},
	// };
	/*
	 * void vkCmdPushDescriptorSetKHR(
	 *     VkCommandBuffer             commandBuffer,
	 *     VkPipelineBindPoint         pipelineBindPoint,
	 *     VkPipelineLayout            layout,
	 *     uint32_t                    set,
	 *     uint32_t                    descriptorWriteCount,
	 *     const VkWriteDescriptorSet *pDescriptorWrites
	 * );
	 */
	/* TODO */
	(void)si;
	const std::array writes = {
		vk::WriteDescriptorSet {},
	};
	// si.compute_command_buffer.pushDescriptorSetKHR(
	//     vk::PipelineBindPoint::eCompute, _sprite_batch_pipe->layout(), 0,
	//     writes)
	si.compute_command_buffer.push_descriptor_set(_sprite_batch_pipe,
	    writes);
}

void
euler::vulkan::Surface::flush_ubo_buffers(Swapchain::Frame &)
{
	/* TODO */
}

euler::util::Reference<euler::vulkan::Renderer>
euler::vulkan::Surface::renderer() const
{
	return _renderer.strengthen();
}

const euler::vulkan::Device &
euler::vulkan::Surface::device() const
{
	return renderer()->device();
}

euler::vulkan::Device &
euler::vulkan::Surface::device()
{
	return renderer()->device();
}

const euler::vulkan::PhysicalDevice &
euler::vulkan::Surface::physical_device() const
{
	return renderer()->physical_device();
}

euler::vulkan::PhysicalDevice &
euler::vulkan::Surface::physical_device()
{
	return renderer()->physical_device();
}
bool
euler::vulkan::Surface::enable_msaa() const
{
	return renderer()->config().msaa > vk::SampleCountFlagBits::e1;
}

void
euler::vulkan::Surface::draw(const std::function<void()> &fn,
    const util::Color clear)
{
	bool started = false;
	try {
		start_frame(clear);
		started = true;
		fn();
		end_frame();
	} catch (...) {
		if (started) end_frame();
		throw;
	}
}

vk::SurfaceCapabilitiesKHR
euler::vulkan::Surface::capabilities() const
{
	return physical_device().physical_device().getSurfaceCapabilitiesKHR(
	    _surface);
}

void
euler::vulkan::Surface::increment_current_frame()
{
	_current_frame = (_current_frame + 1) % Swapchain::FRAMES_IN_FLIGHT;
}

vk::PresentModeKHR
euler::vulkan::Surface::present_mode() const
{
	auto &pdev = physical_device().physical_device();
	for (const auto mode : pdev.getSurfacePresentModesKHR(_surface))
		if (mode == renderer()->config().present_mode) return mode;
	return vk::PresentModeKHR::eFifo;
}
