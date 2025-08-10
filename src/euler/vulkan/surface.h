/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_SURFACE_H
#define EULER_VULKAN_SURFACE_H

#include "euler/util/object.h"

namespace euler::vulkan {
/* Meant to be inherited by graphics::Window */
class Surface : public util::Object {
public:
	virtual SDL_Window *window() const = 0;
	virtual size_t width() const = 0;
	virtual size_t height() const = 0;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_SURFACE_H */
