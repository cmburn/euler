/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_RENDERER_H
#define EULER_VULKAN_RENDERER_H

#ifndef __cplusplus
#include <stdint.h>
#else
#include <thread>

#include "euler/util/logger.h"
#include "euler/util/object.h"
#include "euler/util/version.h"

struct nk_context;

#ifdef EULER_INTERNAL_BUILD
typedef struct VK2DLogger VK2DLogger;
#else
typedef void VK2DLogger; /* opaque */
#endif

namespace euler::vulkan {
class Surface;

/* There can be multiple states per Euler instance, but there can only ever be
 * one renderer. */
class Renderer final : public util::Object {
	friend class Surface;

public:
	Renderer(const util::Reference<util::Logger> &log)
	    : _log(log->copy("vulkan"))
	{
	}
	~Renderer() override;

	void initialize(const util::Reference<Surface> &surface);

	nk_context *gui_context();
	const nk_context *gui_context() const;

private:
	util::Reference<Surface> surface() const;

	util::WeakReference<Surface> _surface;
	util::Reference<util::Logger> _log;
	VK2DLogger *_vk2d_logger = nullptr;
	bool _initialized = false;
};
} /* namespace euler::vulkan */

extern "C" {
#endif

#ifdef __cplusplus
typedef euler::vulkan::Renderer euler_vulkan_renderer;
#else
typedef void euler_vulkan_renderer; /* opaque */
#endif

const uint8_t *euler_vulkan_renderer_shader_data(
    euler_vulkan_renderer *renderer, const char *key, size_t *size_out);

#ifdef __cplusplus
}
#endif

#endif /* EULER_VULKAN_RENDERER_H */
