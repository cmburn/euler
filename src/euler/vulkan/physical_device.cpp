/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/physical_device.h"

#include "euler/vulkan/renderer.h"

euler::vulkan::PhysicalDevice::PhysicalDevice(Renderer *renderer,
    vk::raii::PhysicalDevice &&pdev)
    : _physical_device(std::move(pdev))
    , _renderer(renderer)
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
