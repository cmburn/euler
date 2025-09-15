/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_RENDERER_H
#define EULER_VULKAN_RENDERER_H

#ifndef __cplusplus
#include <stdint.h>
#else
#include <thread>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan_raii.hpp>

#include "euler/util/logger.h"
#include "euler/util/object.h"
#include "euler/util/version.h"
#include "euler/vulkan/common.h"
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
	struct Config {
		util::Version version;
		std::string application;
		std::optional<uint32_t> preferred_gpu;
		vk::SampleCountFlagBits msaa = vk::SampleCountFlagBits::e1;
		vk::PresentModeKHR present_mode = vk::PresentModeKHR::eFifo;
	};
	using ShaderData = std::span<const uint8_t>;

	Renderer(const util::Reference<Surface> &surface, const Config &config);
	std::optional<ShaderData> shader_data(std::string_view key);

	util::Reference<Surface> surface() const;

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

	util::Reference<util::Logger>
	log() const
	{
		return _log;
	}

	~Renderer() override;

private:
	vk::raii::PhysicalDevice select_physical_device();
	PhysicalDevice create_physical_device();
	vk::raii::Device select_device();
	Device create_device();
	static std::optional<ShaderData> load_builtin_shader(
	    std::string_view key);

	std::unordered_map<std::string, std::vector<uint8_t>> _runtime_shaders;
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

extern "C" {
#endif

#ifdef __cplusplus
typedef euler::vulkan::Renderer euler_vulkan_renderer;
#else
typedef void euler_vulkan_renderer; /* opaque */
#endif

const uint8_t *euler_vulkan_renderer_shader_data(
    euler_vulkan_renderer *renderer, const char *key, size_t *size_out);

#ifdef __cplusplus
}
#endif

#endif /* EULER_VULKAN_RENDERER_H */
