/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/camera.h"

#include "euler/vulkan/renderer.h"

euler::vulkan::Camera::Camera(const util::Reference<Surface> &surface,
    const Spec &spec) : _surface(surface)
{
	set_spec(spec);
}
void
euler::vulkan::Camera::set_spec(const Spec &spec)
{
	_spec = spec;
	if (_spec.on_screen.w == 0.0f)
		_spec.on_screen.w = surface()->width();
	if (_spec.on_screen.h == 0.0f)
		_spec.on_screen.h = surface()->height();
}

void
euler::vulkan::Camera::set_state(State state)
{
	surface()->flush_sprite_batch();
	_state = state;
}

void
euler::vulkan::Camera::update_ubo(glm::mat4 &)
{
	/* TODO */
}

euler::util::Reference<euler::vulkan::Surface>
euler::vulkan::Camera::surface() const
{
	return _surface.strengthen();
}
