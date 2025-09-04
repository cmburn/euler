/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/camera.h"

#include "euler/vulkan/renderer.h"

euler::vulkan::Camera::Camera(const util::Reference<Renderer> &renderer,
    const Spec &spec) : _renderer(renderer)
{
	set_spec(spec);
}
void
euler::vulkan::Camera::set_spec(const Spec &spec)
{
	_spec = spec;
	if (_spec.on_screen.w == 0.0f)
		_spec.on_screen.w = renderer()->surface()->width();
	if (_spec.on_screen.h == 0.0f)
		_spec.on_screen.h = renderer()->surface()->height();
}

void
euler::vulkan::Camera::set_state(State state)
{
	renderer()->flush_sprite_batch();
	_state = state;
}

euler::util::Reference<euler::vulkan::Renderer>
euler::vulkan::Camera::renderer() const
{
	return _renderer.strengthen();
}