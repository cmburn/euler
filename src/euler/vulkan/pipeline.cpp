/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/pipeline.h"

const vk::raii::PipelineLayout &
euler::vulkan::Pipeline::layout() const
{
	static vk::raii::PipelineLayout dummy{ nullptr };
	return dummy;
}
vk::raii::PipelineLayout &
euler::vulkan::Pipeline::layout()
{
	static vk::raii::PipelineLayout dummy{ nullptr };
	return dummy;
}