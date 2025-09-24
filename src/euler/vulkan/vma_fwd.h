/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_VMA_FWD_H
#define EULER_VULKAN_VMA_FWD_H

#include <vulkan/vulkan_core.h>

/* I'm aware extern "C" does nothing to types, this is for readability */

extern "C" {
VK_DEFINE_HANDLE(VmaAllocation)
VK_DEFINE_HANDLE(VmaAllocator)
}

#endif /* EULER_VULKAN_VMA_FWD_H */

