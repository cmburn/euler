/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_GRAPHICS_PIPELINE_H
#define EULER_VULKAN_GRAPHICS_PIPELINE_H

#include "euler/vulkan/pipeline.h"

namespace euler::vulkan {
class GraphicsPipeline final : public Pipeline {
public:
	[[nodiscard]] PipelineType
	pipeline_type() const override
	{
		return PipelineType::Graphics;
	}
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_GRAPHICS_PIPELINE_H */
