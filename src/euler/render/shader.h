/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_RENDER_SHADER_H
#define EULER_RENDER_SHADER_H

namespace euler::render {
class Shader : public util::Object {
	Shader(VK2DShader shader);
public:
	static util::Reference<Shader> from_files(
		const std::filesystem::path &vertex,
		const std::filesystem::path &fragment,
		uint32_t uniform_size = 0
		);

	static util::Reference<Shader> from_buffers(
		std::span<const uint8_t> vertex,
		std::span<const uint8_t> fragment,
		uint32_t uniform_size = 0
		);

	~Shader();
private:
	VK2DShader _shader;
};

} /* namespace euler::render */


#endif /* EULER_RENDER_SHADER_H */

