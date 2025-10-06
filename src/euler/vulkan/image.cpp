/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/image.h"

#include "euler/vulkan/surface.h"

euler::vulkan::Image::CreateInfo
euler::vulkan::Image::CreateInfo::empty()
{
	return {};
}

euler::vulkan::Image::Image(const util::Reference<Surface> &surface,
    vk::ImageAspectFlags, vk::ImageUsageFlags, CreateInfo)
    : _surface(surface)
{
	/* TODO */
}

euler::util::Reference<euler::vulkan::Surface>
euler::vulkan::Image::surface() const
{
	return _surface.strengthen();
}