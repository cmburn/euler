/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_SURFACE_H
#define EULER_VULKAN_SURFACE_H

#include <functional>

#include <SDL3/SDL.h>

#include "euler/util/color.h"
#include "euler/util/object.h"
#include "euler/vulkan/camera.h"
#include "euler/vulkan/texture.h"
#include "euler/vulkan/renderer.h"

namespace euler::vulkan {
class Renderer;

class Surface : public util::Object {
	friend class Renderer;
public:
	Surface();
	~Surface() override;
	virtual SDL_Window *window() const = 0;
	virtual uint32_t width() const = 0;
	virtual uint32_t height() const = 0;
	virtual util::Reference<util::Logger> log() const = 0;
	const util::Reference<Renderer> &renderer() const;
	util::Reference<Renderer> &renderer();
	bool draw(int &exit_code, const std::function<bool(int &)> &fn);

	void test_gui();

protected:
	void start_gui_input();
	void end_gui_input();

private:
	void set_renderer(const util::Reference<Renderer> &renderer);
	util::Reference<Renderer> _renderer;
};

} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_SURFACE_H */
