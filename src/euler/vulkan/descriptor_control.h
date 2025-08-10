/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_DESCRIPTOR_CONTROL_H
#define EULER_VULKAN_DESCRIPTOR_CONTROL_H

#include "euler/util/object.h"
#include "euler/vulkan/device.h"
#include "euler/vulkan/texture.h"

namespace euler::vulkan {
class DescriptorControl final : public util::Object {
public:
	DescriptorControl(util::Reference<Device> device,
	    vk::raii::DescriptorSetLayout &&layout,
	    std::optional<uint32_t> buffer = std::nullopt,
	    std::optional<uint32_t> sampler = std::nullopt);

	vk::raii::DescriptorSet buffer_set(util::Reference<Buffer> buffer);
	vk::raii::DescriptorSet sampler_set(util::Reference<Texture> texture);
	vk::raii::DescriptorSet sampler_buffer_set(
	    util::Reference<Texture> texture, util::Reference<Buffer> buffer);
	vk::raii::DescriptorSet descriptor_set();
	void reset();

private:
	util::Reference<Device> _device;
	vk::raii::DescriptorSetLayout _layout;
	std::optional<uint32_t> _buffer;
	std::optional<uint32_t> _sampler;
};
} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_DESCRIPTOR_CONTROL_H */
