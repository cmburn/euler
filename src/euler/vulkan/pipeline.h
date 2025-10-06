/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_PIPELINE_H
#define EULER_VULKAN_PIPELINE_H

#include <vulkan/vulkan_raii.hpp>

#include "euler/util/object.h"

namespace euler::vulkan {

class Pipeline : public util::Object {
public:
	~Pipeline() override = default;

	enum class BlendMode {
		Default,
		None,
		Add,
		Subtract,
	};
	static constexpr uint32_t BLEND_MODE_MAX = 4;


	struct ComputePushBuffer {
		uint32_t draw_count;
	};

	enum class PipelineType : bool { Graphics, Compute };

	virtual PipelineType pipeline_type() const = 0;

	const vk::raii::PipelineLayout &layout() const;
	vk::raii::PipelineLayout &layout();

	vk::raii::Pipeline &pipeline(BlendMode blend_mode = BlendMode::Default)
	{
		const uint32_t n = static_cast<uint32_t>(blend_mode);
		assert(n < BLEND_MODE_MAX);
		return _pipelines.at(n);
	}

protected:
	Pipeline();
	std::array<vk::raii::Pipeline, BLEND_MODE_MAX> _pipelines;
};

} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_PIPELINE_H */
