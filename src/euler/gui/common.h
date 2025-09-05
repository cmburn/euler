/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_COMMON_H
#define EULER_GUI_COMMON_H

namespace euler::gui {

enum class Symbol {
	None,
	X,
	Underscore,
	SolidCircle,
	OutlinedCircle,
	SolidRectangle,
	OutlinedRectangle,
	UpTriangle,
	DownTriangle,
	LeftTriangle,
	RightTriangle,
	Plus,
	Minus,
	OutlinedUpTriangle,
	OutlinedDownTriangle,
	OutlinedLeftTriangle,
	OutlinedRightTriangle,
};

struct Alignment {
	enum class Horizontal : uint8_t {
		Left,
		Center,
		Right
	};
	enum class Vertical : uint8_t {
		Top,
		Middle,
		Bottom
	};
	Horizontal horizontal = Horizontal::Center;
	Vertical vertical = Vertical::Middle;
};


} /* namespace euler::gui */


#endif /* EULER_GUI_COMMON_H */

