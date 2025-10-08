/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_CAMERA_H
#define EULER_RENDERER_CAMERA_H

#include "euler/util/object.h"

namespace euler::graphics {
class Camera final : public util::Object {
public:

private:
	int _index = 0;
};
} /* namespace Euler::Graphics */

#endif /* EULER_RENDERER_CAMERA_H */

