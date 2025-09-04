/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_SHADER_H
#define EULER_VULKAN_SHADER_H

#include "euler/util/object.h"

#include <vulkan/vulkan_raii.hpp>
#include <vector>
#include <filesystem>

namespace euler::vulkan {
class Pipeline;
class Device;

class Shader final : public util::Object {
public:
	Shader(const std::filesystem::path &path, uint32_t uniform_size = 0);
	Shader(const std::vector<uint8_t> &vertex,
	    const std::vector<uint8_t> &fragment, uint32_t uniform_size = 0);

private:
	util::Reference<Pipeline> _pipeline;
	std::vector<uint8_t> _vertex;
	std::vector<uint8_t> _fragment;
	uint32_t _uniform_size;
	Device &_device;
};

} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_SHADER_H */

