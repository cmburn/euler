/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/physical_device.h"

#include "euler/vulkan/renderer.h"

euler::vulkan::PhysicalDevice::PhysicalDevice(Renderer *renderer,
    vk::raii::PhysicalDevice &&pdev)
    : _physical_device(std::move(pdev))
    , _renderer(renderer)
{
	select_queue_families();
}

euler::vulkan::PhysicalDevice::PhysicalDevice(nullptr_t)
    : _physical_device(nullptr)
    , _renderer(nullptr)
{
}

vk::SampleCountFlagBits
euler::vulkan::PhysicalDevice::msaa() const
{
	const auto limits = _physical_device.getProperties().limits;
	const auto counts = limits.framebufferColorSampleCounts
	    & limits.framebufferDepthSampleCounts;
	if (counts & vk::SampleCountFlagBits::e64)
		return vk::SampleCountFlagBits::e64;
	if (counts & vk::SampleCountFlagBits::e32)
		return vk::SampleCountFlagBits::e32;
	if (counts & vk::SampleCountFlagBits::e16)
		return vk::SampleCountFlagBits::e16;
	if (counts & vk::SampleCountFlagBits::e8)
		return vk::SampleCountFlagBits::e8;
	if (counts & vk::SampleCountFlagBits::e4)
		return vk::SampleCountFlagBits::e4;
	if (counts & vk::SampleCountFlagBits::e2)
		return vk::SampleCountFlagBits::e2;
	return vk::SampleCountFlagBits::e1;
}

bool
euler::vulkan::PhysicalDevice::supports_surface(
    const util::Reference<Surface> &surface) const
{
	return _physical_device.getSurfaceSupportKHR(graphics_family(),
	    surface->surface());
}

void
euler::vulkan::PhysicalDevice::select_queue_families()
{
	const auto properties = _physical_device.getQueueFamilyProperties();

	for (uint32_t i = 0; i < properties.size(); i++) {
		const auto &prop = properties[i];
		if (prop.queueFlags & vk::QueueFlagBits::eGraphics
		    && _graphics_family == NO_QUEUE) {
			_graphics_family = i;
		}
		if (prop.queueFlags & vk::QueueFlagBits::eCompute
		    && _compute_family == NO_QUEUE && i != _graphics_family) {
			_compute_family = i;
		}
	}
	if (_graphics_family == std::numeric_limits<uint32_t>::max())
		throw std::runtime_error("Failed to find a graphics queue");
	if (_compute_family == NO_QUEUE)
		_renderer->log()->warn("No compute queue available on device");
}
