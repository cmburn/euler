/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_SHADER_H
#define EULER_RENDERER_SHADER_H
#include <cstdint>
#include <filesystem>
#include <span>

#include "euler/util/object.h"

typedef struct VK2DShader_t *VK2DShader;

namespace Euler::Graphics {
class Shader : public Util::Object {
	Shader(VK2DShader shader);
public:
	static Util::Reference<Shader> from_files(
		const std::filesystem::path &vertex,
		const std::filesystem::path &fragment,
		uint32_t uniform_size = 0
		);

	static Util::Reference<Shader> from_buffers(
		std::span<const uint8_t> vertex,
		std::span<const uint8_t> fragment,
		uint32_t uniform_size = 0
		);

	~Shader();
private:
	VK2DShader _shader;
};
} /* namespace Euler::Graphics */

#endif /* EULER_RENDERER_SHADER_H */

