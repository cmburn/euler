/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_PIPELINE_H
#define EULER_VULKAN_PIPELINE_H

#include <vulkan/vulkan_raii.hpp>

#include "euler/util/object.h"

namespace euler::vulkan {

class Pipeline : public util::Object {
public:
	~Pipeline() override = default;

	struct ComputePushBuffer {
		uint32_t draw_count;
	};

	vk::raii::Pipeline &
	compute_pipeline()
	{
		return _pipelines[0];
	}

	const vk::raii::Pipeline &
	compute_pipeline() const
	{
		return _pipelines[0];
	}

	const vk::raii::PipelineLayout &layout() const;
	vk::raii::PipelineLayout &layout();

protected:
	Pipeline();
	std::array<vk::raii::Pipeline, 4> _pipelines;
};

} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_PIPELINE_H */
