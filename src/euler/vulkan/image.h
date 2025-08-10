/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_IMAGE_H
#define EULER_VULKAN_IMAGE_H

#include <filesystem>

#include <vulkan/vulkan.hpp>

#include "SDL3/SDL_surface.h"
#include "euler/util/object.h"
#include "euler/vulkan/device.h"

namespace euler::vulkan {
class Image final : public util::Object {
public:
	struct Info {
		uint32_t width;
		uint32_t height;
		vk::Format format;
		vk::ImageAspectFlags aspect_mask;
		vk::ImageUsageFlags usage;
		vk::SampleCountFlags samples;
	};
	Image(util::Reference<Device> device, const Info &info);
	Image(util::Reference<Device> device, std::filesystem::path path);
	Image(util::Reference<Device> device, SDL_Surface *surface);
	~Image() override;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_IMAGE_H */
