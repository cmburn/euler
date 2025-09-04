/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_RENDERER_H
#define EULER_VULKAN_RENDERER_H

#include <thread>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan_raii.hpp>

#include "euler/util/logger.h"
#include "euler/util/object.h"
#include "euler/util/version.h"
#include "euler/vulkan/device.h"
#include "euler/vulkan/physical_device.h"
#include "euler/vulkan/surface.h"
#include "euler/vulkan/texture.h"

namespace euler::vulkan {
/* There can be multiple states per Euler instance, but there should be only
 * one renderer,
 */
class Renderer final : public util::Object {
	friend class Surface;

public:
	struct Config {
		util::Version version;
		std::string application;
		std::optional<uint32_t> preferred_gpu;
	};

	Renderer(const util::Reference<Surface> &surface, const Config &config);

	util::Reference<Surface> surface() const;
	void set_target(util::Reference<Texture> target);
	void flush_sprite_batch();
	bool
	on_render_thread() const
	{
		return std::this_thread::get_id() == _render_thread;
	}

	const Config &
	config() const
	{
		return _config;
	}

	VmaAllocator
	allocator() const
	{
		return _allocator;
	}

private:
	vk::raii::PhysicalDevice select_physical_device();
	PhysicalDevice create_physical_device();
	vk::raii::Device select_device();
	Device create_device();
	// vk::AllocationCallbacks *allocator();

	std::thread::id _render_thread;
	vk::raii::Context _context = {};
	vk::raii::Instance _instance;
	Config _config;
	util::Reference<Surface> _surface;
	util::Reference<util::Logger> _log;
	PhysicalDevice _physical_device;
	Device _device;
	uint32_t _graphics_queue_index = 0;
	uint32_t _present_queue_index = 0;
	uint32_t _compute_queue_index = 0;
	VmaAllocator _allocator = nullptr;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_RENDERER_H */
