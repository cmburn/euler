/* SPDX-License-Identifier: ISC */

#ifndef EULER_GRAPHICS_COLOR_H
#define EULER_GRAPHICS_COLOR_H

#include <cstdint>
#include <SDL3/SDL_pixels.h>

#include "euler/util/object.h"

namespace euler::graphics {

class Color {
public:
	constexpr Color(const SDL_Color color)
	    : _red(color.r)
	    , _green(color.g)
	    , _blue(color.b)
	    , _alpha(color.a)
	{
	}

	constexpr Color(const uint32_t color)
	    : _red((color >> 24) & 0xFF)
	    , _green((color >> 16) & 0xFF)
	    , _blue((color >> 8) & 0xFF)
	    , _alpha(color & 0xFF)
	{
	}

	constexpr Color(const uint8_t red, const uint8_t green,
	    const uint8_t blue, const uint8_t alpha = SDL_ALPHA_OPAQUE)
	    : _red(red)
	    , _green(green)
	    , _blue(blue)
	    , _alpha(alpha)
	{
	}

	[[nodiscard]] uint8_t
	red() const
	{
		return _red;
	}

	[[nodiscard]] uint8_t
	green() const
	{
		return _green;
	}

	[[nodiscard]] uint8_t
	blue() const
	{
		return _blue;
	}

	[[nodiscard]] uint8_t
	alpha() const
	{
		return _alpha;
	}

	[[nodiscard]] SDL_Color
	to_sdl() const
	{
		return SDL_Color { _red, _green, _blue, _alpha };
	}

	[[nodiscard]] uint32_t
	to_uint32() const
	{
		return (_red << 24) | (_green << 16) | (_blue << 8) | _alpha;
	}

	void
	set_red(const uint8_t red)
	{
		_red = red;
	}

	void
	set_green(const uint8_t green)
	{
		_green = green;
	}

	void
	set_blue(const uint8_t blue)
	{
		_blue = blue;
	}

	void
	set_alpha(const uint8_t alpha)
	{
		_alpha = alpha;
	}

	static constexpr Color WHITE { 255, 255, 255, 255 };
	static constexpr Color BLACK { 0, 0, 0, 255 };
	static constexpr Color RED { 255, 0, 0, 255 };
	static constexpr Color GREEN { 0, 255, 0, 255 };
	static constexpr Color BLUE { 0, 0, 255, 255 };

private:
	uint8_t _red;
	uint8_t _green;
	uint8_t _blue;
	uint8_t _alpha;
};

} /* namespace euler::graphics */

#endif /* EULER_GRAPHICS_COLOR_H */
