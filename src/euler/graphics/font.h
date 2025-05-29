/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_FONT_H
#define EULER_RENDERER_FONT_H

#include <cstdint>
#include <filesystem>
#include <optional>
#include <variant>
#include <vector>

#include "Eigen/Core"

#include "euler/util/object.h"

struct VK2DGuiFont;
struct nk_font_config;

namespace Euler::Graphics {
class Gui;

class Font final : public Util::Object {
	friend class Gui;

public:
	struct Config {
		std::vector<std::pair<uint32_t, uint32_t>> glyph_ranges;
		Eigen::Vector2f spacing;
		uint32_t fallback_glyph;
	};

	Font(const std::string &name, const std::filesystem::path &path,
	    const float height, const Config *config)
	    : _config(*config)
	    , _data(path)
	    , _name(name)
	    , _height(height)
	{
	}

	Font(const std::string &name, const std::string &path,
	    const float height, const Config *config)
	    : Font(name, std::filesystem::path(path), height, config)
	{
	}

	Font(const std::string &name, const std::vector<uint8_t> &data,
	    const float height, const Config *config)
	    : _config(*config)
	    , _data(data)
	    , _name(name)
	    , _height(height)
	{
	}

private:
	friend class Gui;
	void populate_info(VK2DGuiFont *font, nk_font_config *config);

	std::optional<Config> _config;
	std::variant<std::filesystem::path, std::vector<uint8_t>> _data;
	std::string _name;
	float _height = 0.0f;
};
} /* namespace Euler::Graphics */

#endif /* EULER_RENDERER_FONT_H */
