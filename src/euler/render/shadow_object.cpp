/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#include "euler/render/shadow_object.h"


void
euler::graphics::ShadowObject::set_position(Eigen::Vector2f position)
{
	vk2dShadowEnvironmentObjectSetPos(environment()->_shadow_environment,
	    _value, position.x(), position.y());
}

void
euler::graphics::ShadowObject::update(Eigen::Vector2f position,
    Eigen::Vector2f scale, Eigen::Vector2f origin, const float angle)
{
	vk2dShadowEnvironmentObjectUpdate(environment()->_shadow_environment,
	    _value, position.x(), position.y(), scale.x(), scale.y(), angle,
	    origin.x(), origin.y());
}

void
euler::graphics::ShadowObject::set_visible(const bool visible)
{
	vk2dShadowEnvironmentObjectSetStatus(environment()->_shadow_environment,
	    _value, visible);
}
bool
euler::graphics::ShadowObject::visible() const
{
	return vk2dShadowEnvironmentObjectGetStatus(
	    environment()->_shadow_environment, _value);
}

euler::util::Reference<euler::graphics::ShadowEnvironment>
euler::graphics::ShadowObject::environment() const
{
	return _environment.strengthen();
}
