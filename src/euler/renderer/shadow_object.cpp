/* SPDX-License-Identifier: ISC */

#include "euler/renderer/shadow_object.h"

#include "VK2D/ShadowEnvironment.h"
#include "euler/renderer/shadow_environment.h"

void
Euler::Renderer::ShadowObject::set_position(Eigen::Vector2f position)
{
	vk2dShadowEnvironmentObjectSetPos(_environment->_shadow_environment,
	    _value, position.x(), position.y());
}

void
Euler::Renderer::ShadowObject::update(Eigen::Vector2f position,
    Eigen::Vector2f scale, Eigen::Vector2f origin, const float angle)
{
	vk2dShadowEnvironmentObjectUpdate(_environment->_shadow_environment,
	    _value, position.x(), position.y(), scale.x(), scale.y(), angle,
	    origin.x(), origin.y());
}

void
Euler::Renderer::ShadowObject::set_visible(const bool visible)
{
	vk2dShadowEnvironmentObjectSetStatus(_environment->_shadow_environment,
	    _value, visible);
}
bool
Euler::Renderer::ShadowObject::visible() const
{
	return vk2dShadowEnvironmentObjectGetStatus(
	    _environment->_shadow_environment, _value);
}