/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/renderer.h"

#include <vk_mem_alloc.h>

euler::vulkan::Renderer::Renderer(util::Reference<Instance> instance)
    : _instance(instance)
{
	
}

euler::vulkan::Renderer::~Renderer() { vmaDestroyAllocator(_allocator); }