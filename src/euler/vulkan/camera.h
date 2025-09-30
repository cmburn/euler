/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_CAMERA_H
#define EULER_VULKAN_CAMERA_H

#include <glm/fwd.hpp>

#include "euler/util/object.h"

namespace euler::vulkan {
class Renderer;
class Surface;
class DescriptorBuffer;

class Camera final : public util::Object {
public:
	using Index = uint16_t;

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

	Camera(const util::Reference<Surface> &surface, const Spec &spec);
	~Camera() override = default;
	void set_spec(const Spec &spec);
	const Spec &
	spec() const
	{
		return _spec;
	}

	State state() const { return _state; }
	void flush_ubo(DescriptorBuffer &buffer);
	void set_state(State state);
	glm::mat4 ubo() const;
	[[nodiscard]] util::Reference<Surface> surface() const;

	Index index() const
	{
		return _index;
	}

private:
	Spec _spec;
	Index _index = 0;
	util::WeakReference<Surface> _surface;
	State _state = State::Normal;
};
} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_CAMERA_H */

