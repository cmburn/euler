/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_RENDERER_H
#define EULER_VULKAN_RENDERER_H

#include <SDL3/SDL_vulkan.h>
#include <glm/glm.hpp>

#include "euler/util/object.h"
#include "euler/vulkan/camera.h"
#include "euler/vulkan/device.h"
#include "euler/vulkan/instance.h"
#include "euler/vulkan/physical_device.h"
#include "euler/vulkan/surface.h"
#include "euler/vulkan/swapchain.h"
#include "euler/vulkan/texture.h"

typedef struct VmaAllocator_T *VmaAllocator;

namespace euler::vulkan {
class Renderer final : public util::Object {
public:
	struct Config { };

	Renderer(util::Reference<Instance> instance);

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

	VmaAllocator
	allocator() const
	{
		return _allocator;
	}

	util::Reference<Device> device() const;
	util::Reference<Surface> surface() const;
	void set_target(util::Reference<Texture> target);
	void flush_sprite_batch();

private:
	std::unordered_map<Camera::Index, util::Reference<Camera>> _cameras;
	util::Reference<PhysicalDevice> _physical_device;
	util::Reference<Device> _device;
	util::Reference<Surface> _surface;
	util::Reference<Instance> _instance;
	util::Reference<Swapchain> _swapchain;
	VmaAllocator _allocator = nullptr;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_RENDERER_H */
