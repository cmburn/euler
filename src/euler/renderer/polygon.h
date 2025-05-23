/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_POLYGON_H
#define EULER_RENDERER_POLYGON_H

#include <Eigen/Core>

typedef struct VK2DPolygon_t *VK2DPolygon;

namespace Euler::Renderer {
class Polygon {
public:
	struct VertexColor {
		Eigen::Vector3f position;
		Eigen::Vector4f color;
	};
	Polygon(const std::vector<VertexColor> &vertices);
	Polygon(const std::vector<Eigen::Vector2f> &vertices, bool filled = false);
	~Polygon();
private:
	VK2DPolygon _polygon = nullptr;
};
} /* namespace Euler::Renderer */

#endif /* EULER_RENDERER_POLYGON_H */

