/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_SURFACE_H
#define EULER_VULKAN_SURFACE_H

#include <SDL3/SDL.h>
#include <vulkan/vulkan_raii.hpp>

#include "euler/util/object.h"

namespace euler::vulkan {
class Renderer;

/* Meant to be inherited by graphics::Window */
class Surface : public util::Object {
	friend class Renderer;
public:
	virtual SDL_Window *window() const = 0;
	virtual size_t width() const = 0;
	virtual size_t height() const = 0;
	virtual util::Reference<util::Logger> log() const = 0;

	const vk::raii::SurfaceKHR &surface() const
	{
		return _swapchain->_surface;
	}
	vk::raii::SurfaceKHR &surface()
	{
		return _swapchain->_surface;
	}

	~Surface() override = default;

private:
	void rebuild_swapchain();
	void initialize_swapchain(Renderer *renderer);

	struct Swapchain {
		vk::raii::SurfaceKHR _surface;
		vk::raii::SwapchainKHR _swapchain;
		vk::Format _format;
	};
	/* Functionally, this should have a value unless we're in startup, but
	 * due to initialization order we have to keep this as an optional for
	 * RAII reasons. */
	std::optional<Swapchain> _swapchain;
};

} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_SURFACE_H */
