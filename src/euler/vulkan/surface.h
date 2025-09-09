/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_SURFACE_H
#define EULER_VULKAN_SURFACE_H

#include <SDL3/SDL.h>
#include <vulkan/vulkan_raii.hpp>
#include <glm/glm.hpp>

#include "euler/util/object.h"
#include "euler/vulkan/camera.h"
#include "euler/vulkan/swapchain.h"
#include "euler/vulkan/texture.h"

namespace euler::vulkan {
class Renderer;
class Device;

/* Meant to be inherited by graphics::Window */
class Surface : public util::Object {
	friend class Renderer;

public:
	virtual SDL_Window *window() const = 0;
	virtual size_t width() const = 0;
	virtual size_t height() const = 0;
	virtual util::Reference<util::Logger> log() const = 0;
	Surface();

	struct RenderTarget {
		vk::raii::Framebuffer *frame = nullptr;
		vk::raii::RenderPass *render_pass = nullptr;
		uint32_t subpass = 0;
		vk::raii::Image *image = nullptr;
		vk::raii::DescriptorSet *ubo_set = nullptr;
		util::Reference<Texture> target = nullptr;
		void
		clear()
		{
			frame = nullptr;
			render_pass = nullptr;
			subpass = 0;
			image = nullptr;
			ubo_set = nullptr;
			target = nullptr;
		}
	};

	const vk::raii::SurfaceKHR &
	surface() const
	{
		return _surface;
	}
	vk::raii::SurfaceKHR &
	surface()
	{
		return _surface;
	}

	void initialize_vulkan(const util::Reference<Renderer> &renderer);
	void start_frame(RenderTarget &rt);
	void end_frame();
	void flush_ubo_buffers(Swapchain::Frame &frame);
	util::Reference<Renderer> renderer() const;

	~Surface() override = default;

private:
	void rebuild_swapchain();
	void initialize_swapchain(Renderer *renderer);
	void
	increment_current_frame()
	{
		_current_frame
		    = (_current_frame + 1) % Swapchain::FRAMES_IN_FLIGHT;
	}

	// struct Swapchain {
	// 	vk::raii::SurfaceKHR _surface;
	// 	vk::raii::SwapchainKHR _swapchain;
	// 	vk::Format _format;
	// };
	// /* Functionally, this should have a value unless we're in startup,
	// but
	//  * due to initialization order we have to keep this as an optional
	//  for
	//  * RAII reasons. */
	// std::optional<Swapchain> _swapchain;

	struct CameraInfo {
		util::Reference<Camera> camera;
		glm::mat4 projection;
	};
	vk::raii::SurfaceKHR _surface;
	Swapchain _swapchain;
	util::WeakReference<Renderer> _renderer;
	std::vector<CameraInfo> _cameras;
	uint32_t _swapchain_image_index = 0;
	uint32_t _current_frame = 0;
	bool _need_rebuild = false;
};

} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_SURFACE_H */
