/* SPDX-License-Identifier: ISC */

#include "euler/graphics/shader.h"

#include "VK2D/Shader.h"

Euler::Graphics::Shader::Shader(const VK2DShader shader)
    : _shader(shader)
{
}

Euler::Util::Reference<Euler::Graphics::Shader>
Euler::Graphics::Shader::from_files(const std::filesystem::path &vertex,
    const std::filesystem::path &fragment, const uint32_t uniform_size)
{
	if (!std::filesystem::exists(vertex)
	    || !std::filesystem::exists(fragment)) {
		throw std::runtime_error("Shader file does not exist");
	}
	const auto shader = vk2dShaderLoad(vertex.string().c_str(),
	    fragment.string().c_str(), uniform_size);
	if (shader == nullptr)
		throw std::runtime_error("Failed to create shader");
	const auto s = new Shader(shader);
	return Util::Reference(s);
}

Euler::Util::Reference<Euler::Graphics::Shader>
Euler::Graphics::Shader::from_buffers(std::span<const uint8_t> vertex,
    std::span<const uint8_t> fragment, uint32_t uniform_size)
{
	if (vertex.empty() || fragment.empty())
		throw std::runtime_error("Shader buffer is empty");
	const auto shader = vk2dShaderFrom(vertex.data(), vertex.size(),
	    fragment.data(), fragment.size(), uniform_size);
	if (shader == nullptr)
		throw std::runtime_error("Failed to create shader");
	const auto s = new Shader(shader);
	return Util::Reference(s);
}

Euler::Graphics::Shader::~Shader()
{
	if (_shader != nullptr) vk2dShaderFree(_shader);
}