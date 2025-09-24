/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_SURFACE_H
#define EULER_VULKAN_SURFACE_H

#include <functional>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "euler/util/color.h"
#include "euler/util/object.h"
#include "euler/vulkan/camera.h"
#include "euler/vulkan/pipeline.h"
#include "euler/vulkan/swapchain.h"
#include "euler/vulkan/texture.h"

namespace euler::vulkan {
class Renderer;
class Device;
class PhysicalDevice;

/* Meant to be inherited by graphics::Window */
class Surface : public util::Object {
	friend class Renderer;
	friend class Swapchain;

public:
	virtual SDL_Window *window() const = 0;
	virtual uint32_t width() const = 0;
	virtual uint32_t height() const = 0;
	virtual util::Reference<util::Logger> log() const = 0;
	virtual vk::raii::Semaphore gui_render() const = 0;
	Surface();

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

	void initialize_vulkan(const util::Reference<Renderer> &r);
	void start_frame(util::Color clear = util::BLACK);
	void end_frame();
	void flush_sprite_batch();
	void flush_ubo_buffers(Swapchain::Frame &frame);
	util::Reference<Renderer> renderer() const;
	const Device &device() const;
	Device &device();
	const PhysicalDevice &physical_device() const;
	PhysicalDevice &physical_device();
	bool enable_msaa() const;

	Swapchain &
	swapchain()
	{
		return _swapchain;
	}

	const Swapchain &
	swapchain() const
	{
		return _swapchain;
	}

	uint32_t
	current_frame() const
	{
		return _current_frame;
	}

	uint32_t
	image_index() const
	{
		return _image_index;
	}

	const vk::raii::Semaphore &
	render_finished() const
	{
		return _swapchain.render_finished();
	}

	vk::raii::Semaphore &
	render_finished()
	{
		return _swapchain.render_finished();
	}

	void draw(const std::function<void()> &fn, util::Color = util::BLACK);

	vk::SurfaceCapabilitiesKHR capabilities() const;

	const vk::SurfaceFormatKHR &
	format() const
	{
		return _format;
	}

	void
	set_format(const vk::SurfaceFormatKHR &format)
	{
		_format = format;
	}

	vk::Extent2D
	extent() const
	{
		return { width(), height() };
	}

	std::vector<std::string> instance_extensions() const;

	~Surface() override = default;

	struct DrawCommand {
		glm::vec4 texture_position;
		glm::vec4 color;
		glm::vec2 position;
		glm::vec2 origin;
		glm::vec2 scale;
		float rotation;
		uint32_t texture_index;
	};

	struct DrawInstance { };

private:
	void rebuild_swapchain();
	void destroy_swapchain();
	void initialize_swapchain();
	void increment_current_frame();

	struct CameraInfo {
		util::Reference<Camera> camera;
		glm::mat4 projection;
	};

	[[nodiscard]] vk::PresentModeKHR present_mode() const;
	vk::raii::SurfaceKHR _surface;
	Swapchain _swapchain;
	util::WeakReference<Renderer> _renderer;
	std::vector<CameraInfo> _cameras;
	util::Reference<Pipeline> _sprite_batch_pipe;
	vk::SurfaceFormatKHR _format = {
		.format = vk::Format::eB8G8R8A8Srgb,
		.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear,
	};
	std::vector<DrawCommand> _draw_commands;
	std::vector<DrawInstance> _draw_instances;

	uint32_t _image_index = 0;
	uint32_t _current_frame = 0;
	bool _need_rebuild = false;
};

} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_SURFACE_H */
