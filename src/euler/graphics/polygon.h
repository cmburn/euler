/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_POLYGON_H
#define EULER_RENDERER_POLYGON_H

#include <glm/glm.hpp>

typedef struct VK2DPolygon_t *VK2DPolygon;

namespace euler::graphics {
class Polygon {
public:
	struct VertexColor {
		glm::vec3 position;
		glm::vec4 color;
	};
	Polygon(const std::vector<VertexColor> &vertices);
	Polygon(const std::vector<glm::vec2> &vertices, bool filled = false);
	~Polygon();
private:
	VK2DPolygon _polygon = nullptr;
};
} /* namespace Euler::Graphics */

#endif /* EULER_RENDERER_POLYGON_H */

