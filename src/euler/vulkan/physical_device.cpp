/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/physical_device.h"

#include "euler/vulkan/renderer.h"

euler::vulkan::PhysicalDevice::PhysicalDevice(Renderer *renderer,
    vk::raii::PhysicalDevice &&pdev)
    : _physical_device(std::move(pdev))
    , _renderer(renderer)
{
}

euler::vulkan::MSAA
euler::vulkan::PhysicalDevice::msaa() const
{
	const auto limits = _physical_device.getProperties().limits;
	const auto counts = limits.framebufferColorSampleCounts
	    & limits.framebufferDepthSampleCounts;
	if (counts & vk::SampleCountFlagBits::e64) return MSAA::x64;
	if (counts & vk::SampleCountFlagBits::e32) return MSAA::x32;
	if (counts & vk::SampleCountFlagBits::e16) return MSAA::x16;
	if (counts & vk::SampleCountFlagBits::e8) return MSAA::x8;
	if (counts & vk::SampleCountFlagBits::e4) return MSAA::x4;
	if (counts & vk::SampleCountFlagBits::e2) return MSAA::x2;
	return MSAA::x1;
}
