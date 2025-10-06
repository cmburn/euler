/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_SWAPCHAIN_H
#define EULER_VULKAN_SWAPCHAIN_H

#include <memory>

#include <vulkan/vulkan_raii.hpp>

#include "euler/vulkan/command_buffer.h"
#include "euler/vulkan/descriptor_buffer.h"
#include "euler/vulkan/image.h"

namespace euler::vulkan {
class Surface;

class Swapchain final {
	friend class Surface;

public:
	static constexpr uint32_t FRAMES_IN_FLIGHT = 2;
	Swapchain();

	void set_surface(const util::Reference<Surface> &surface);

	const vk::raii::SwapchainKHR &
	swapchain() const
	{
		return _swapchain;
	}

	vk::raii::SwapchainKHR &
	swapchain()
	{
		return _swapchain;
	}

	util::Reference<Surface> surface() const;
	const vk::raii::Semaphore &render_finished() const;
	vk::raii::Semaphore &render_finished();

	void rebuild();

private:
	struct SwapchainImage {
		vk::raii::Image image;
		vk::raii::ImageView view;
		vk::raii::Framebuffer framebuffer;
		vk::raii::Fence fence;
		vk::raii::Fence *image_in_flight;
		CommandBuffer command_buffer;
		CommandBuffer db_command_buffer;
		CommandBuffer compute_command_buffer;
		SwapchainImage(vk::Image &&vk_image,
		    const util::Reference<Surface> &surface);

	private:
		vk::raii::ImageView make_image_view(
		    const util::Reference<Surface> &surface) const;
	};

	struct Frame {
		vk::raii::Semaphore image_ready;
		vk::raii::Fence in_flight;
		vk::raii::Semaphore render_complete;
		vk::raii::DescriptorSet ubo_descriptor_set;
		DescriptorBuffer descriptor_buffer;
	};

	uint32_t supported_image_count() const;
	void rebuild_swapchain();
	void rebuild_images();
	void rebuild_render_passes();
	void rebuild_pipelines();
	vk::PresentModeKHR select_present_mode() const;

	uint32_t _rebuild_count = 0;
	vk::raii::SwapchainKHR _swapchain;
	std::vector<SwapchainImage> _images;
	std::vector<Frame> _frames;
	// std::optional<Image> _msaa_image;
	vk::raii::RenderPass _render_pass;
	vk::raii::RenderPass _mid_frame_render_pass;
	vk::raii::RenderPass _external_target_render_pass;


	util::WeakReference<Surface> _surface;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_SWAPCHAIN_H */
