/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_POLYGON_SHAPE_H
#define EULER_PHYSICS_POLYGON_SHAPE_H

#include "euler/physics/shape.h"

namespace euler::physics {
class PolygonShape : public Shape {
public:
	Vec2D point(size_t i) const;
	[[nodiscard]] std::vector<Vec2D> points() const;
	[[nodiscard]] bool is_convex() const;
};
} /* namespace euler::physics */


#endif /* EULER_PHYSICS_POLYGON_SHAPE_H */

