/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_RENDER_TEXTURE_H
#define EULER_RENDER_TEXTURE_H

namespace euler::render {
class Texture : public util::Object {
	Texture(VK2DTexture texture, VK2DImage image = nullptr);

public:
	Texture() = default;
	~Texture() override;

	static util::Reference<Texture> from_pixels(const SDL_Surface *surface);
	static util::Reference<Texture> from_pixels(
	    std::span<const uint32_t> pixels,
	    int width, int height);
	static util::Reference<Texture> from_file(const std::string &filename) {
		return from_file(std::filesystem::path(filename));
	}
	static util::Reference<Texture> from_file(
	    const std::filesystem::path &filename);
	static util::Reference<Texture> from_memory(std::span<const uint8_t> data);

	[[nodiscard]] float width() const;
	[[nodiscard]] float height() const;
	[[nodiscard]] bool is_target() const;
	[[nodiscard]] uint32_t id() const;

private:
	VK2DTexture _texture = nullptr;
	/* Only needed when loaded via pixels */
	VK2DImage _image = nullptr;
};

} /* namespace euler::render */


#endif /* EULER_RENDER_TEXTURE_H */

