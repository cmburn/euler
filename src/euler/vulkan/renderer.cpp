/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/renderer.h"
#include "SDL3/SDL_events.h"

namespace util = euler::util;

static vk::raii::Instance
make_instance(const vk::raii::Context &context,
    const euler::vulkan::Renderer::Config &config)
{
	const vk::ApplicationInfo app_info {
		.pApplicationName = config.application.c_str(),
		.applicationVersion = config.version.to_vulkan(),
		.pEngineName = "euler",
		.engineVersion = util::version().to_vulkan(),
		.apiVersion = vk::ApiVersion14,
	};
	const vk::InstanceCreateInfo create_info {
		.pApplicationInfo = &app_info,
	};

	return vk::raii::Instance(context, create_info);
}

euler::vulkan::Renderer::Renderer(util::Reference<Window> window,
    const Config &config)
    : _instance(make_instance(_context, config))
    , _window(window)
{
}

// void
// euler::vulkan::Renderer::run(std::function<bool()> fn)
// {
// 	for (bool done = false; !done;) {
// 		SDL_Event event;
// 		while (SDL_PollEvent(&event)) {
// 			if (event.type == SDL_EVENT_QUIT) {
// 				done = true;
// 			}
// 		}
// 		done = fn();
// 	}
// }