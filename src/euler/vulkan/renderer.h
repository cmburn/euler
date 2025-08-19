/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_RENDERER_H
#define EULER_VULKAN_RENDERER_H

#include <functional>

#include <vulkan/vulkan_raii.hpp>

#include "euler/util/object.h"
#include "euler/util/version.h"
#include "euler/vulkan/window.h"

namespace euler::vulkan {
class Renderer final : public util::Object {
public:
	struct Config {
		util::Version version;
		std::string application;
	};

	Renderer(util::Reference<Window> window,
		const Config& config);

	void run(std::function<void()> fn);

private:
	vk::raii::Context _context;
	vk::raii::Instance _instance;
	util::Reference<Window> _window;
};
} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_RENDERER_H */

