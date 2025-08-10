/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_CAMERA_H
#define EULER_VULKAN_CAMERA_H

#include "euler/util/object.h"

namespace euler::vulkan {
class Renderer;

class Camera final : public util::Object {
public:
	enum class State {
		Normal,
		Disabled,
		Deleted,
		Reset,
	};
	struct Spec {
		float x;
		float y;
		float w;
		float h;
		float zoom;
		float rotation;
		struct {
			float x;
			float y;
			float w;
			float h;
		} on_screen;
	};
	Camera(util::Reference<Renderer> renderer, const Spec &spec);
	~Camera() override;
	void set_spec(const Spec &spec);
	const Spec &
	spec() const
	{
		return _spec;
	}
	/* ReSharper disable once CppHidingFunction */
	State state() const { return _state; }
	void set_state(State state);
	util::Reference<Renderer> renderer() const;

private:
	Spec _spec;
	util::WeakReference<Renderer> _renderer;
	State _state = State::Normal;
};
} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_CAMERA_H */

