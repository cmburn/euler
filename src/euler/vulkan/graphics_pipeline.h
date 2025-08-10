/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_GRAPHICS_PIPELINE_H
#define EULER_VULKAN_GRAPHICS_PIPELINE_H

#include "euler/vulkan/pipeline.h"

namespace euler::vulkan {
class GraphicsPipeline final : public Pipeline {
public:
	~GraphicsPipeline() override = default;
};
} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_GRAPHICS_PIPELINE_H */

