/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_TEXTURE_H
#define EULER_VULKAN_TEXTURE_H

#include <filesystem>
#include <vulkan/vulkan_raii.hpp>

#include "euler/util/object.h"

namespace euler::vulkan {
class Image;

class Texture final : public util::Object {
public:
	Texture(Image &&image);
	Texture(const std::filesystem::path &path);
	~Texture() override;

};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_TEXTURE_H */
