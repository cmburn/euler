/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_TYPES_H
#define EULER_VULKAN_TYPES_H

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

namespace euler::vulkan {

enum class MSAA {
	x1 = vk::SampleCountFlagBits::e1,
	x2 = vk::SampleCountFlagBits::e2,
	x4 = vk::SampleCountFlagBits::e4,
	x8 = vk::SampleCountFlagBits::e8,
	x16 = vk::SampleCountFlagBits::e16,
	x32 = vk::SampleCountFlagBits::e32,
	x64 = vk::SampleCountFlagBits::e64,
};

// struct ColorVertex {
// 	glm::vec4 position;
// 	glm::vec4 color;
// };

using ColorVertex = glm::mat4x2;
using Data = std::span<uint8_t>;

} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_TYPES_H */

