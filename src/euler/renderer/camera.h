/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_CAMERA_H
#define EULER_RENDERER_CAMERA_H

namespace Euler::Renderer {
class Camera {
public:
	enum class Type {
		Default,
		Orthographic,
		Perspective,
	};

	struct Spec {
		Type type;
		float x;
		float y;
		float w;
		float h;
		float zoom;
		float rot;
		float x_on_screen;
		float y_on_screen;
		float w_on_screen;
		float h_on_screen;
	};

	enum class State {
		Normal,
		Disabled,
		Deleted,
		Reset,
	};

	Camera(Spec spec);

	void update(Spec spec);
	Spec spec();
	void set_state(State state);
	State state();


private:
	int _index = 0;
};
} /* namespace Euler::Renderer */

#endif /* EULER_RENDERER_CAMERA_H */

