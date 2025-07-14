/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_PHYSICS_SHAPE_H
#define EULER_PHYSICS_SHAPE_H

#include "euler/physics/body.h"
#include "euler/physics/types.h"
#include "euler/util/object.h"

namespace euler::physics {
class Shape : public util::Object {
public:
	enum class Type {
		Chain,
		Circle,
		Edge,
		Polygon,
	};

	struct Translation {
		float x = 0.0f;
		float y = 0.0f;
		float theta = 0.0f; // rotation in radians
	};

	struct AABB {
		Vec2D top_left = { 0.0, 0.0 };
		Vec2D bottom_right = { 0.0, 0.0 };
	};

	struct Ray {
		Vec2D input_start = { 0.0, 0.0 };
		Vec2D input_end = { 0.0, 0.0 };
		float max_fraction = 1.0f;
		Translation translation = { 0.0f, 0.0f };
	};

	struct Intercept {
		Vec2D normal_vector = { 0.0, 0.0 };
		float fraction = 1.0f;
	};

	[[nodiscard]] AABB compute_aabb(Translation t, size_t child_idx) const;
	[[nodiscard]] float compute_mass(float density);
	[[nodiscard]] size_t child_count() const;
	[[nodiscard]] virtual float radius() const;
	[[nodiscard]] virtual Type type() const = 0;
	[[nodiscard]] Intercept ray_cast(Ray ray, size_t child_idx) const;
};
} /* namespace euler::physics */

#endif /* EULER_PHYSICS_SHAPE_H */
