/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_EDGE_SHAPE_H
#define EULER_PHYSICS_EDGE_SHAPE_H

#include "euler/physics/shape.h"

namespace euler::physics {
class EdgeShape : public Shape {
public:
	void set_previous_vertex(Vec2D v);
	[[nodiscard]] Vec2D previous_vertex() const;
	void set_next_vertex(Vec2D v);
	[[nodiscard]] Vec2D next_vertex() const;
	[[nodiscard]] size_t vertex_count() const;
	Vec2D point(size_t i) const;
	[[nodiscard]] std::vector<Vec2D> points() const;

};
} /* namespace euler::physics */


#endif /* EULER_PHYSICS_EDGE_SHAPE_H */

