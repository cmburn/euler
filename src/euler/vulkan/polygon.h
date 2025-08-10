/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_POLYGON_H
#define EULER_VULKAN_POLYGON_H

#include <glm/glm.hpp>

#include "euler/vulkan/types.h"
#include "euler/util/object.h"

namespace euler::vulkan {
class Polygon final : public util::Object {
public:
	Polygon(const std::vector<glm::vec2> &vertices, bool outline = false);
	Polygon(const std::vector<ColorVertex> &vertices);

};
} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_POLYGON_H */

