/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/swapchain.h"

#include "euler/vulkan/renderer.h"
#include "euler/vulkan/surface.h"

euler::vulkan::Swapchain::Swapchain()
    : _swapchain(nullptr)
    , _render_pass(nullptr)
    , _mid_frame_render_pass(nullptr)
    , _external_target_render_pass(nullptr)
    , _surface(nullptr)
{
}

void
euler::vulkan::Swapchain::set_surface(const util::Reference<Surface> &surface)
{
	_surface = surface.weaken();
}

euler::util::Reference<euler::vulkan::Surface>
euler::vulkan::Swapchain::surface() const
{
	return _surface.strengthen();
}

const vk::raii::Semaphore &
euler::vulkan::Swapchain::render_finished() const
{
	return _frames[surface()->current_frame()].render_complete;
}

vk::raii::Semaphore &
euler::vulkan::Swapchain::render_finished()
{
	return _frames[surface()->current_frame()].render_complete;
}

void
euler::vulkan::Swapchain::rebuild()
{
	rebuild_swapchain();
	rebuild_images();
	rebuild_render_passes();
}

euler::vulkan::Swapchain::SwapchainImage::SwapchainImage(vk::Image &&vk_image,
    const util::Reference<Surface> &surface)
    : image(surface->renderer()->device().device(), std::move(vk_image))
    , view(make_image_view(surface))
    , framebuffer(nullptr)
    , fence(nullptr)
    , image_in_flight(nullptr)
{
}

vk::raii::ImageView
euler::vulkan::Swapchain::SwapchainImage::make_image_view(
    const util::Reference<Surface> &surface) const
{
	const vk::ImageViewCreateInfo ci = {
		.image = &**image,
		.viewType = vk::ImageViewType::e2D,
		.format = surface->format().format,
		.components = {
			.r = vk::ComponentSwizzle::eIdentity,
			.g = vk::ComponentSwizzle::eIdentity,
			.b = vk::ComponentSwizzle::eIdentity,
			.a = vk::ComponentSwizzle::eIdentity,
		},
		.subresourceRange = {
			.aspectMask = vk::ImageAspectFlagBits::eColor,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1,
		},
	};
	return surface->device().device().createImageView(ci);
}

uint32_t
euler::vulkan::Swapchain::supported_image_count() const
{
	const auto min = surface()->capabilities().minImageCount;
	const auto max = surface()->capabilities().maxImageCount;
	const uint32_t count = std::max(min, 3u);
	if (count > max) return max;
	return count;
}

void
euler::vulkan::Swapchain::rebuild_swapchain()
{
	const auto rebuilding = _rebuild_count++ != 0;
	const auto image_count = supported_image_count();
	const auto surf = surface();
	auto &dev = surf->renderer()->device();
	if (rebuilding) {
		//_swapchain = nullptr;
		_images.clear();
		_frames.clear();
		_render_pass = nullptr;
		_mid_frame_render_pass = nullptr;
		_external_target_render_pass = nullptr;
	}
	_images.reserve(image_count);
	_frames.reserve(image_count);
	const vk::SwapchainCreateInfoKHR create_info = {
		.surface = *(surf->surface()),
		.minImageCount = image_count,
		.imageFormat = surf->format().format,
		.imageColorSpace = surf->format().colorSpace,
		.imageExtent = surf->extent(),
		.imageArrayLayers = 0,
		.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
		.imageSharingMode = vk::SharingMode::eExclusive,
		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices = nullptr,
		.preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity,
		.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
		.presentMode = surf->present_mode(),
		.clipped = true,
		.oldSwapchain = rebuilding ? *_swapchain : nullptr,
	};
	assert(surf->renderer()->physical_device().supports_surface(surf));
	_swapchain = dev.device().createSwapchainKHR(create_info);
}

void
euler::vulkan::Swapchain::rebuild_images()
{
	auto images = _swapchain.getImages();
	assert(images.size() == supported_image_count());
	for (auto &&image : images)
		_images.emplace_back(std::move(image), surface());
	if (surface()->enable_msaa()) {
		_msaa_image = Image {
			surface(),
			vk::ImageAspectFlagBits::eColor,
			vk::ImageUsageFlagBits::eColorAttachment,
		};
	}
}
void
euler::vulkan::Swapchain::rebuild_render_passes()
{
	std::array attachments = {
		vk::AttachmentDescription {
		    .format = surface()->format().format,
		    .samples = surface()->renderer()->config().msaa,
		    .loadOp = vk::AttachmentLoadOp::eClear,
		    .storeOp = vk::AttachmentStoreOp::eStore,
		    .stencilLoadOp = vk::AttachmentLoadOp::eLoad,
		    .stencilStoreOp = vk::AttachmentStoreOp::eStore,
		    .initialLayout = vk::ImageLayout::eUndefined,
		    .finalLayout = surface()->enable_msaa()
			? vk::ImageLayout::eColorAttachmentOptimal
			: vk::ImageLayout::ePresentSrcKHR,
		},
		vk::AttachmentDescription {
		    .format = surface()->format().format,
		    .samples = vk::SampleCountFlagBits::e1,
		    .loadOp = vk::AttachmentLoadOp::eDontCare,
		    .storeOp = vk::AttachmentStoreOp::eStore,
		    .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
		    .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
		    .initialLayout = vk::ImageLayout::eUndefined,
		    .finalLayout = vk::ImageLayout::ePresentSrcKHR,
		},
	};
	const uint32_t attachment_count = surface()->enable_msaa() ? 2U : 1U;
	static constexpr vk::AttachmentReference resolve_attachment = {
		.attachment = 1,
		.layout = vk::ImageLayout::eColorAttachmentOptimal,
	};
	static constexpr std::array subpass_color_attachments = {
		vk::AttachmentReference {
		    .attachment = 0,
		    .layout = vk::ImageLayout::eColorAttachmentOptimal,
		},
	};
	std::array subpasses = {
		vk::SubpassDescription {
		    .pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
		    .colorAttachmentCount
		    = static_cast<uint32_t>(subpass_color_attachments.size()),
		    .pColorAttachments = subpass_color_attachments.data(),
		    .pResolveAttachments
		    = surface()->enable_msaa() ? &resolve_attachment : nullptr,
		    .pDepthStencilAttachment = nullptr,
		},
	};
	static constexpr vk::SubpassDependency subpass_dependency = {
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		.srcStageMask
		= vk::PipelineStageFlagBits::eColorAttachmentOutput,
		.dstStageMask
		= vk::PipelineStageFlagBits::eColorAttachmentOutput,
		.srcAccessMask = vk::AccessFlags(),
		.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite,
	};
	const vk::RenderPassCreateInfo rp_ci = {
		.attachmentCount = attachment_count,
		.pAttachments = attachments.data(),
		.subpassCount = static_cast<uint32_t>(subpasses.size()),
		.pSubpasses = subpasses.data(),
		.dependencyCount = 1,
		.pDependencies = &subpass_dependency,
	};
	_render_pass = surface()->device().device().createRenderPass(rp_ci);
	if (surface()->enable_msaa()) {
		attachments[0].initialLayout
		    = vk::ImageLayout::eColorAttachmentOptimal;
		attachments[0].finalLayout
		    = vk::ImageLayout::eColorAttachmentOptimal;
		attachments[0].loadOp = vk::AttachmentLoadOp::eLoad;
		attachments[1].initialLayout = vk::ImageLayout::ePresentSrcKHR;
		attachments[1].finalLayout = vk::ImageLayout::ePresentSrcKHR;
		attachments[1].loadOp = vk::AttachmentLoadOp::eLoad;
	} else {
		attachments[0].initialLayout = vk::ImageLayout::ePresentSrcKHR;
		attachments[0].finalLayout = vk::ImageLayout::ePresentSrcKHR;
		attachments[0].loadOp = vk::AttachmentLoadOp::eLoad;
	}
	_mid_frame_render_pass
	    = surface()->device().device().createRenderPass(rp_ci);
	attachments[0].initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
	attachments[0].finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
	attachments[0].loadOp = vk::AttachmentLoadOp::eLoad;
	if (surface()->enable_msaa()) {
		attachments[1].initialLayout
		    = vk::ImageLayout::eColorAttachmentOptimal;
		attachments[1].finalLayout
		    = vk::ImageLayout::eColorAttachmentOptimal;
		attachments[1].loadOp = vk::AttachmentLoadOp::eLoad;
	}
	_external_target_render_pass
	    = surface()->device().device().createRenderPass(rp_ci);
}
