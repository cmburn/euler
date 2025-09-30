/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/camera.h"

#include "euler/vulkan/renderer.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

euler::vulkan::Camera::Camera(const util::Reference<Surface> &surface,
    const Spec &spec)
    : _surface(surface)
{
	set_spec(spec);
}
void
euler::vulkan::Camera::set_spec(const Spec &spec)
{
	_spec = spec;
	if (_spec.on_screen.w == 0.0f) _spec.on_screen.w = surface()->width();
	if (_spec.on_screen.h == 0.0f) _spec.on_screen.h = surface()->height();
}

void
euler::vulkan::Camera::set_state(State state)
{
	surface()->flush_sprite_batch();
	_state = state;
}

glm::mat4
euler::vulkan::Camera::ubo() const
{
	const glm::vec3 eyes = {
		_spec.x + (_spec.w * 0.5f),
		_spec.y + (_spec.h * 0.5f),
		-2.0f,
	};
	const glm::vec3 center = {
		eyes.x,
		eyes.y,
		0.0f,
	};
	const glm::vec3 up = {
		sin(-_spec.rotation),
		-cos(-_spec.rotation),
		0.0f,
	};
	const auto view = glm::lookAt(eyes, center, up);
	const auto proj = glm::ortho(-_spec.w / 2.0f * _spec.zoom,
	    _spec.w / 2.0f * _spec.zoom, -_spec.h / 2.0f * _spec.zoom,
	    _spec.h / 2.0f * _spec.zoom, 0.1f, 10.0f);
	return proj * view;
}

euler::util::Reference<euler::vulkan::Surface>
euler::vulkan::Camera::surface() const
{
	return _surface.strengthen();
}
