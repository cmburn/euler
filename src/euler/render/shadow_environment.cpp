/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#include "euler/render/shadow_environment.h"

euler::graphics::ShadowEnvironment::ShadowEnvironment()
    : _shadow_environment(vk2dShadowEnvironmentCreate())
{
	if (_shadow_environment == nullptr)
		throw std::runtime_error("Failed to create shadow environment");
}

euler::graphics::ShadowEnvironment::~ShadowEnvironment()
{
	if (_shadow_environment != nullptr)
		vk2dShadowEnvironmentFree(_shadow_environment);
}

euler::util::Reference<euler::graphics::ShadowObject>
euler::graphics::ShadowEnvironment::add_object()
{
	auto obj = vk2dShadowEnvironmentAddObject(_shadow_environment);
	if (obj == VK2D_INVALID_SHADOW_OBJECT)
		throw std::runtime_error("Failed to add shadow object");
	const auto self = util::Reference(this);
	const auto so = new ShadowObject(self.weaken(), obj);
	return util::Reference(so);
}