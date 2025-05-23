/* SPDX-License-Identifier: ISC */

#include <stdexcept>

#include "VK2D/Constants.h"
#include "VK2D/ShadowEnvironment.h"
#include "euler/renderer/shadow_environment.h"

Euler::Renderer::ShadowEnvironment::ShadowEnvironment()
    : _shadow_environment(vk2dShadowEnvironmentCreate())
{
	if (_shadow_environment == nullptr)
		throw std::runtime_error("Failed to create shadow environment");
}

Euler::Renderer::ShadowEnvironment::~ShadowEnvironment()
{
	if (_shadow_environment != nullptr)
		vk2dShadowEnvironmentFree(_shadow_environment);
}

Euler::Util::Reference<Euler::Renderer::ShadowObject>
Euler::Renderer::ShadowEnvironment::add_object()
{
	auto obj = vk2dShadowEnvironmentAddObject(_shadow_environment);
	if (obj == VK2D_INVALID_SHADOW_OBJECT)
		throw std::runtime_error("Failed to add shadow object");
	const auto self = Util::Reference(this);
	const auto so = new ShadowObject(self.weaken(), obj);
	return Util::Reference(so);
}