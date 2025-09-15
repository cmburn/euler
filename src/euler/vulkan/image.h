/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_IMAGE_H
#define EULER_VULKAN_IMAGE_H

#include <vulkan/vulkan_raii.hpp>
#include <optional>

#include "euler/util/object.h"

namespace euler::vulkan {
class Surface;

class Image final {
public:
	/* Any nullopt values will default to the value read from the passed-in
	 * surface. */
	struct CreateInfo {
		std::optional<uint32_t> width = std::nullopt;
		std::optional<uint32_t> height = std::nullopt;
		std::optional<uint32_t> levels = std::nullopt;
		std::optional<vk::Format> format = std::nullopt;
		/* separate function for default constructor reasons*/
		static CreateInfo empty();
	};

	Image(const util::Reference<Surface> &surface,
		vk::ImageAspectFlags aspect,
		vk::ImageUsageFlags usage,
		CreateInfo info = CreateInfo::empty());

	util::Reference<Surface> surface() const;

private:
	util::WeakReference<Surface> _surface;
};
} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_IMAGE_H */

