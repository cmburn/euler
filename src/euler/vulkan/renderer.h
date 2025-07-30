/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_RENDERER_H
#define EULER_VULKAN_RENDERER_H

#include <glm/glm.hpp>
#include <SDL3/SDL_vulkan.h>

#include "euler/util/object.h"
#include "euler/vulkan/surface.h"
#include "euler/vulkan/device.h"
#include "euler/vulkan/texture.h"

typedef struct VmaAllocator_T *VmaAllocator;

namespace euler::vulkan {
class Renderer final : util::Object {
public:
	struct Config {};

	~Renderer() override;

	void wait() const;

	struct VRAMUsage {
		float in_use;
		float total;
	};

	VRAMUsage vram_usage() const;
	void reset_swapchain();
	void start_frame();
	void end_frame();
	VmaAllocator allocator() const
	{
		return _allocator;
	}

	util::Reference<Device> device() const;
	void set_target(util::Reference<Texture> target);
private:
	util::Reference<Surface> _surface;
	VmaAllocator _allocator = nullptr;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_RENDERER_H */

