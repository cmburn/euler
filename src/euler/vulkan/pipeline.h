/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_PIPELINE_H
#define EULER_VULKAN_PIPELINE_H

#include <vulkan/vulkan_raii.hpp>

#include "euler/util/object.h"

namespace euler::vulkan {
class Pipeline : public util::Object {
public:
	~Pipeline() override = default;

	vk::raii::Pipeline &pipeline() { return _pipeline; }
	const vk::raii::Pipeline &pipeline() const { return _pipeline; }

protected:
	vk::raii::Pipeline _pipeline;
};
} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_PIPELINE_H */

