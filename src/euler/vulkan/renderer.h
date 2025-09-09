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
/* There can be multiple states per Euler instance, but there can only ever be
 * one renderer. */
class Renderer final : public util::Object {
	friend class Surface;

public:
	static constexpr auto FRAMES_IN_FLIGHT = Swapchain::FRAMES_IN_FLIGHT;
	using RenderTarget = Surface::RenderTarget;
	struct Config {
		util::Version version;
		std::string application;
		std::optional<uint32_t> preferred_gpu;
	};

	Renderer(const util::Reference<Surface> &surface, const Config &config);

	util::Reference<Surface> surface() const;
	void set_target(util::Reference<Texture> target);
	void flush_sprite_batch();

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

	const PhysicalDevice &
	physical_device() const
	{
		return _physical_device;
	}

	const Device &
	device() const
	{
		return _device;
	}

	PhysicalDevice &
	physical_device()
	{
		return _physical_device;
	}

	Device &
	device()
	{
		return _device;
	}

	util::Reference<util::Logger> log() const { return _log; }

	void frame(const std::function<void()> &fn);

	~Renderer() override;


private:
	void start_frame();
	void end_frame();
	vk::raii::PhysicalDevice select_physical_device();
	PhysicalDevice create_physical_device();
	vk::raii::Device select_device();
	Device create_device();

	std::binary_semaphore _frame_semaphore = std::binary_semaphore(1);
	vk::raii::Context _context = {};
	vk::raii::Instance _instance;
	Config _config;
	util::Reference<Surface> _surface;
	util::Reference<util::Logger> _log;
	PhysicalDevice _physical_device;
	Device _device;
	RenderTarget _current_target;
	std::array<vk::Fence, FRAMES_IN_FLIGHT> _fences;
	uint32_t _graphics_queue_index = 0;
	uint32_t _present_queue_index = 0;
	uint32_t _compute_queue_index = 0;

	VmaAllocator _allocator = nullptr;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_RENDERER_H */
