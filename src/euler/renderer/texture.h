/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_TEXTURE_H
#define EULER_RENDERER_TEXTURE_H
#include <cstdint>
#include <filesystem>
#include <span>
#include <string>
#include <vector>

#include "euler/util/object.h"
#include "vulkan/vulkan.hpp"

/* ReSharper disable CppRedundantElaboratedTypeSpecifier */
typedef struct VK2DTexture_t *VK2DTexture;
typedef struct VK2DImage_t *VK2DImage;
/* ReSharper enable CppRedundantElaboratedTypeSpecifier */

namespace Euler::Renderer {
class Texture : public Util::Object {
	Texture(VK2DTexture texture, VK2DImage image = nullptr);

public:
	Texture() = default;
	~Texture() override;

	static Util::Reference<Texture> from_pixels(const SDL_Surface *surface);
	static Util::Reference<Texture> from_pixels(
	    std::span<const uint32_t> pixels,
	    int width, int height);
	static Util::Reference<Texture> from_file(const std::string &filename) {
		return from_file(std::filesystem::path(filename));
	}
	static Util::Reference<Texture> from_file(
	    const std::filesystem::path &filename);
	static Util::Reference<Texture> from_memory(std::span<const uint8_t> data);

	[[nodiscard]] float width() const;
	[[nodiscard]] float height() const;
	[[nodiscard]] bool is_target() const;
	[[nodiscard]] uint32_t id() const;

private:
	VK2DTexture _texture = nullptr;
	/* Only needed when loaded via pixels */
	VK2DImage _image = nullptr;
};
} /* namespace Euler::Renderer */

#endif /* EULER_RENDERER_TEXTURE_H */
