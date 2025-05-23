/* SPDX-License-Identifier: ISC */

#include <fstream>

#include "VK2D/Image.h"
#include "VK2D/Opaque.h"
#include "VK2D/Renderer.h"
#include "VK2D/Texture.h"
#include "euler/renderer/texture.h"
#include "euler/util/thread.h"

Euler::Renderer::Texture::~Texture()
{
	if (_texture != nullptr) vk2dTextureFree(_texture);
	if (_image != nullptr) vk2dImageFree(_image);
}

Euler::Util::Reference<Euler::Renderer::Texture>
Euler::Renderer::Texture::from_pixels(const SDL_Surface *surface)
{
	assert(surface->format == SDL_PIXELFORMAT_RGBA32);
	const int w = surface->w;
	const int h = surface->h;
	const void *pixels = surface->pixels;
	const bool main_thread = Util::on_main_thread();
	const auto vk2d = vk2dRendererGetPointer();
	auto image = vk2dImageFromPixels(vk2d->ld, pixels, w, h, main_thread);
	if (image == nullptr)
		throw std::runtime_error("Failed to create image from pixels");
	auto texture = vk2dTextureLoadFromImage(image);
	if (texture == nullptr) {
		vk2dImageFree(image);
		image = nullptr;
		throw std::runtime_error("Failed to create texture from image");
	}
	const auto t = new Texture(texture, image);
	return Util::Reference(t);
}

Euler::Util::Reference<Euler::Renderer::Texture>
Euler::Renderer::Texture::from_pixels(const std::span<const uint32_t> pixels,
    const int width, const int height)
{
	assert(pixels.size() == static_cast<size_t>(width * height));
	const bool main_thread = Util::on_main_thread();
	const auto vk2d = vk2dRendererGetPointer();
	auto image = vk2dImageFromPixels(vk2d->ld, pixels.data(), width, height,
	    main_thread);
	if (image == nullptr)
		throw std::runtime_error("Failed to create image from pixels");
	auto texture = vk2dTextureLoadFromImage(image);
	if (texture == nullptr) {
		vk2dImageFree(image);
		image = nullptr;
		throw std::runtime_error("Failed to create texture from image");
	}
	const auto t = new Texture(texture, image);
	return Util::Reference(t);
}

Euler::Util::Reference<Euler::Renderer::Texture>
Euler::Renderer::Texture::from_file(const std::filesystem::path &filename)
{
	if (!std::filesystem::exists(filename))
		throw std::runtime_error("File does not exist");
	if (const auto texture = vk2dTextureLoad(filename.string().c_str());
	    texture != nullptr) {
		const auto t = new Texture(texture);
		return Util::Reference(t);
	}
	throw std::runtime_error("Failed to create texture from file");
}

Euler::Util::Reference<Euler::Renderer::Texture>
Euler::Renderer::Texture::from_memory(const std::span<const uint8_t> data)
{
	assert(data.size() <= INT_MAX);
	const auto texture
	    = vk2dTextureFrom(data.data(), static_cast<int>(data.size()));
	if (texture == nullptr) {
		throw std::runtime_error(
		    "Failed to create texture from memory");
	}
	const auto t = new Texture(texture);
	return Util::Reference(t);
}

float
Euler::Renderer::Texture::width() const
{
	return vk2dTextureWidth(_texture);
}

float
Euler::Renderer::Texture::height() const
{
	return vk2dTextureHeight(_texture);
}

bool
Euler::Renderer::Texture::is_target() const
{
	return vk2dTextureIsTarget(_texture);
}

uint32_t
Euler::Renderer::Texture::id() const
{
	return vk2dTextureGetID(_texture);
}