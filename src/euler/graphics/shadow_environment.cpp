/* SPDX-License-Identifier: ISC */

#include <stdexcept>

#include "VK2D/Constants.h"
#include "VK2D/ShadowEnvironment.h"
#include "euler/graphics/shadow_environment.h"

Euler::Graphics::ShadowEnvironment::ShadowEnvironment()
    : _shadow_environment(vk2dShadowEnvironmentCreate())
{
	if (_shadow_environment == nullptr)
		throw std::runtime_error("Failed to create shadow environment");
}

Euler::Graphics::ShadowEnvironment::~ShadowEnvironment()
{
	if (_shadow_environment != nullptr)
		vk2dShadowEnvironmentFree(_shadow_environment);
}

Euler::Util::Reference<Euler::Graphics::ShadowObject>
Euler::Graphics::ShadowEnvironment::add_object()
{
	auto obj = vk2dShadowEnvironmentAddObject(_shadow_environment);
	if (obj == VK2D_INVALID_SHADOW_OBJECT)
		throw std::runtime_error("Failed to add shadow object");
	const auto self = Util::Reference(this);
	const auto so = new ShadowObject(self.weaken(), obj);
	return Util::Reference(so);
}