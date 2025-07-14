/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_CIRCLE_SHAPE_H
#define EULER_PHYSICS_CIRCLE_SHAPE_H

#include "euler/physics/shape.h"

namespace euler::physics {
class CircleShape : public Shape {
public:
	[[nodiscard]] Vec2D point() const;
	[[nodiscard]] float radius() const override;
	[[nodiscard]] Type type() const override
	{
		return Type::Circle;
	}
	void set_radius(float r);
};
} /* namespace euler::physics */


#endif /* EULER_PHYSICS_CIRCLE_SHAPE_H */

