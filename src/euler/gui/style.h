/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_STYLE_H
#define EULER_GUI_STYLE_H

#include "euler/graphics/color.h"
#include "euler/graphics/font.h"
#include "euler/gui/common.h"
#include "euler/gui/image.h"
#include "euler/util/object.h"

namespace euler::gui {

/* Each GUI instance is associated with a Style. */
struct Style final {
public:
	using Color = graphics::Color;
	struct ColorTable {
		Color text;
		Color window;
		Color header;
		Color border;
		Color button;
		Color button_hover;
		Color button_active;
		Color toggle;
		Color toggle_hover;
		Color toggle_cursor;
		Color select;
		Color select_active;
		Color slider;
		Color slider_cursor;
		Color slider_cursor_hover;
		Color slider_cursor_active;
		Color property;
		Color edit;
		Color edit_cursor;
		Color combo;
		Color chart;
		Color chart_color;
		Color chart_color_highlight;
		Color scrollbar;
		Color scrollbar_cursor;
		Color scrollbar_cursor_hover;
		Color scrollbar_cursor_active;
		Color tab_header;
		Color knob;
		Color knob_cursor;
		Color knob_cursor_hover;
		Color knob_cursor_active;
	};

	struct Item {
		std::variant<util::Reference<Image>, graphics::Color> data;
		std::optional<glm::u16vec4> bounds;
	};

	struct Text {
		Color color;
		glm::vec2 padding = { 0.0f, 0.0f };
	};

	struct Button {
		Item normal;
		Item hover;
		Item active;
		Color border_color;
		Color text_background;
		Color text_normal;
		Color text_hover;
		Color text_active;
		Alignment text_alignment;
		float border = 0.0f;
		float rounding = 0.0f;
		glm::vec2 padding = { 0.0f, 0.0f };
		glm::vec2 image_padding = { 0.0f, 0.0f };
		glm::vec2 touch_padding = { 0.0f, 0.0f };
	};

	struct Toggle {
		Item normal;
		Item hover;
		Item active;
		Color border_color;
		Item cursor_normal;
		Item cursor_hover;
		Color text_normal;
		Color text_hover;
		Color text_active;
		Color text_background;
		Alignment text_alignment;
		glm::vec2 padding = { 0.0f, 0.0f };
		glm::vec2 touch_padding = { 0.0f, 0.0f };
		float spacing = 0.0f;
		float border = 0.0f;
	};

	struct Selectable {
		/* background (inactive) */
		Item normal;
		Item hover;
		Item pressed;

		/* background (active) */
		Item normal_active;
		Item hover_active;
		Item pressed_active;

		/* text color (inactive) */
		Color text_normal;
		Color text_hover;
		Color text_pressed;

		/* text color (active) */
		Color text_normal_active;
		Color text_hover_active;
		Color text_pressed_active;
		Color text_background;
		Alignment text_alignment;

		float rounding = 0.0f;
		glm::vec2 padding = { 0.0f, 0.0f };
		glm::vec2 touch_padding = { 0.0f, 0.0f };
		glm::vec2 image_padding = { 0.0f, 0.0f };
	};

	struct Slider {
		/* background */
		Item normal;
		Item hover;
		Item active;
		Color border_color;

		/* background bar */
		Color bar_normal;
		Color bar_active;
		Color bar_filled;

		/* cursor */
		Item cursor_normal;
		Item cursor_hover;
		Item cursor_active;

		/* properties */
		float border = 0.0f;
		float rounding = 0.0f;
		float bar_height = 0.0f;
		glm::vec2 padding = { 0.0f, 0.0f };
		glm::vec2 spacing = { 0.0f, 0.0f };
		glm::vec2 cursor_size = { 0.0f, 0.0f };
	};

	struct Knob {
		Item normal;
		Item hover;
		Item active;
		Color border_color;

		Color knob_normal;
		Color knob_hover;
		Color knob_active;
		Color knob_border_color;

		Color cursor_normal;
		Color cursor_hover;
		Color cursor_active;

		float border = 0.0f;
		float knob_border = 0.0f;
		glm::vec2 padding = { 0.0f, 0.0f };
		glm::vec2 spacing = { 0.0f, 0.0f };
	};

	struct ProgressBar {
		Item normal;
		Item active;
		Item hover;
		Color border_color;

		Item cursor_normal;
		Item cursor_hover;
		Item cursor_active;
		Color cursor_border_color;

		float rounding;
		float border;
		float cursor_border;
		float cursor_rounding;
		glm::vec2 padding = { 0.0f, 0.0f };
	};

	struct Property {
		
	};

	struct TextEdit { };
	struct Chart { };
	struct ScollBar { };
	struct Tab { };
	struct Dropdown { };
	struct Window { };

	ColorTable color_table;
	util::Reference<graphics::Font> font;
	Text text;
	Button button;
	Button contextual_button;
	Button menu_button;
	Toggle option;
	Toggle checkbox;
	Selectable selectable;
	Slider slider;
	Knob knob;
	ProgressBar progress_bar;
	Property property;
	TextEdit text_edit;
	Chart chart;
	ScollBar scroll_horizontal;
	ScollBar scroll_vertical;
	Tab tab;
	Dropdown dropdown;
	Window window;
};

} /* namespace euler::gui */

#endif /* EULER_GUI_STYLE_H */
