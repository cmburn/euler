/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_WINDOW_H
#define EULER_VULKAN_WINDOW_H

#include <SDL3/SDL_video.h>

#include "euler/util/object.h"

namespace euler::vulkan {
class Window : public util::Object {
public:
	virtual SDL_Window *window() const = 0;
	virtual size_t width() const = 0;
	virtual size_t height() const = 0;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_WINDOW_H */
