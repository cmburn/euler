/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_PHYSICS_BODY_H
#define EULER_PHYSICS_BODY_H

#include <vector>

#include "euler/physics/contact.h"
#include "euler/physics/fixture.h"
#include "euler/physics/joint.h"
#include "euler/physics/types.h"
#include "euler/util/object.h"

namespace euler::physics {
class World;

class Body : public util::Object {
public:
	enum class Type {
		Static,
		Kinematic,
		Dynamic,
	};

	void apply_angular_impulse(float impulse);
	void apply_force(Vec2D force, Vec2D point, bool local = true);
	void apply_impulse(Vec2D impulse, Vec2D point, bool local = true);
	void apply_torque(float torque);
	void destroy();

	[[nodiscard]] float theta() const;
	[[nodiscard]] float angular_dampening() const;
	[[nodiscard]] float angular_velocity() const;
	[[nodiscard]] std::vector<util::Reference<Contact>> contacts() const;
	[[nodiscard]] std::vector<util::Reference<Fixture>> fixtures() const;
	[[nodiscard]] float gravity_scale() const;
	[[nodiscard]] float rotational_inertia() const;
	[[nodiscard]] std::vector<util::Reference<Joint>> joints() const;
	[[nodiscard]] float dampening() const;
	[[nodiscard]] Vec2D velocity() const;
	[[nodiscard]] Vec2D velocity_at(Vec2D point, bool local = true) const;
	[[nodiscard]] Vec2D center_of_mass(bool local = true) const;
	[[nodiscard]] Vec2D to_point(Vec2D point, bool local = true) const;
	[[nodiscard]] Vec2D to_vector(Vec2D vector, bool local = true) const;
	[[nodiscard]] float mass() const;
	[[nodiscard]] Vec2D position() const;
	[[nodiscard]] Type type() const;
	[[nodiscard]] util::Reference<Object> user_data() const;
	[[nodiscard]] util::Reference<World> world() const;
	[[nodiscard]] util::Reference<Contact> contact() const;
	[[nodiscard]] bool is_active() const;
	[[nodiscard]] bool is_awake() const;
	[[nodiscard]] bool is_bullet() const;
	[[nodiscard]] bool is_destroyed() const;
	[[nodiscard]] bool is_fixed_rotation() const;
	[[nodiscard]] bool is_sleeping_allowed() const;
	[[nodiscard]] bool is_touching() const;

	void reset_mass_data();
	void set_active(bool active);
	void set_sleeping_allowed(bool allow);
	void set_theta(float theta);
	void set_angular_dampening(float dampening);
	void set_angular_velocity(float velocity);
	void set_awake(bool awake);
	void set_bullet(bool bullet);
	void set_fixed_rotation(bool fixed_rotation);
	void set_gravity_scale(float gravity);
	void set_rotational_inertia(float inertia);
	void set_dampening(float dampening);
	void set_velocity(Vec2D velocity);
	void set_mass(float mass);
	void set_position(Vec2D position);
	void set_type(Type type);
	void set_user_data(util::Reference<Object> object);
};
} /* namespace euler::physics */

#endif /* EULER_PHYSICS_BODY_H */
