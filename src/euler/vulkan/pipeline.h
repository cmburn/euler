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

	enum class PipelineType : bool { Graphics, Compute };

	virtual PipelineType pipeline_type() const = 0;

	const vk::raii::PipelineLayout &layout() const;
	vk::raii::PipelineLayout &layout();

protected:
	Pipeline();
	vk::raii::Pipeline _pipeline;
};

} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_PIPELINE_H */
