/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/surface.h"

#include "euler/vulkan/renderer.h"

euler::vulkan::Surface::Surface()
    : _surface(nullptr)
    , _swapchain(*this)
{
}

void
euler::vulkan::Surface::initialize_vulkan(
    const util::Reference<Renderer> &renderer)
{
	_renderer = renderer.weaken();
}

void
euler::vulkan::Surface::start_frame(RenderTarget &rt)
{
	auto &cf = _swapchain._frames[_current_frame];

	auto &dev = renderer()->device().device();

	if (dev.waitForFences({ 1, &*cf.in_flight }, true, UINT64_MAX)
	    != vk::Result::eSuccess) {
		renderer()->log()->fatal("Failed to wait for image fence");
	}

	// vk::raii::
	const vk::AcquireNextImageInfoKHR ani_info {
		.swapchain = *_swapchain._swapchain,
		.timeout = UINT64_MAX,
		.semaphore = *cf.image_ready,
		.fence = nullptr,
		.deviceMask = _swapchain_image_index,
	};

	if (const auto [result, index] = dev.acquireNextImage2KHR(ani_info);
	    result == vk::Result::eSuccess) {
		_swapchain_image_index = index;
	} else {
		renderer()->log()->fatal("Failed to acquire next image");
	}
	auto &si = _swapchain._images[_swapchain_image_index];
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
	rt.subpass = 0;
	rt.frame = &si.framebuffer;
	rt.render_pass = &_swapchain._render_pass;
	rt.image = &si.image;
	rt.ubo_set = &cf.ubo_descriptor_set;
	rt.target = nullptr; /* screen */

	cf.descriptor_buffer.begin_frame();

	for (auto &ci : _cameras) ci.camera->update_ubo(ci.projection);
	flush_ubo_buffers(cf);


}

void
euler::vulkan::Surface::end_frame()
{
}

euler::util::Reference<euler::vulkan::Renderer>
euler::vulkan::Surface::renderer() const
{
	return _renderer.strengthen();
}