/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_INSTANCE_H
#define EULER_VULKAN_INSTANCE_H

#include <vulkan/vulkan_raii.hpp>

#include "euler/util/object.h"

namespace euler::vulkan {
class Instance final : public util::Object {
public:
	Instance();
	vk::raii::Instance &instance();
	const vk::raii::Instance &instance() const;

private:
	vk::raii::Instance _instance;
	vk
};
} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_INSTANCE_H */

