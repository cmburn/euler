/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_STYLE_H
#define EULER_GUI_STYLE_H

#include "euler/graphics/font.h"
#include "euler/graphics/image.h"
#include "euler/gui/common.h"
#include "euler/util/color.h"
#include "euler/util/object.h"

namespace euler::gui {
using Color = util::Color;
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
	Color dropdown;
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

/* Each GUI instance is associated with a Style. */
struct Style final {
	struct Item {
		std::variant<util::Reference<graphics::Image>, Color> data;
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

	struct Scrollbar {
		Item normal;
		Item hover;
		Item active;
		Color border_color;
		Item cursor_normal;
		Item cursor_hover;
		Item cursor_active;
		Color cursor_border_color;
		float border = 0.0f;
		float rounding = 0.0f;
		float border_cursor = 0.0f;
		float rounding_cursor = 0.0f;
		glm::vec2 padding = { 0.0f, 0.0f };
	};

	struct TextEdit {
		Item normal;
		Item hover;
		Item active;
		Color border_color;

		Scrollbar scrollbar;

		Color cursor_normal;
		Color cursor_hover;
		Color cursor_text_normal;
		Color cursor_text_hover;
		Color text_normal;
		Color text_hover;
		Color text_active;
		Color selected_normal;
		Color selected_hover;
		Color selected_text_normal;
		Color selected_text_hover;
		float border = 0.0f;
		float rounding = 0.0f;
		float cursor_size = 1.0f;
		glm::vec2 scrollbar_size = { 0.0f, 0.0f };
		glm::vec2 padding = { 0.0f, 0.0f };
		float row_padding = 0.0f;
	};

	struct Property {
		Item normal;
		Item hover;
		Item active;
		Color border_color;

		Color label_normal;
		Color label_hover;
		Color label_active;

		Symbol sym_left;
		Symbol sym_right;

		float border = 0.0f;
		float rounding = 0.0f;
		glm::vec2 padding = { 0.0f, 0.0f };
		TextEdit exit;
		Button increase_button;
		Button decrease_button;
	};

	struct Chart {
		Item background;
		Color border_color;
		Color selected_color;
		Color color;
		float border = 0.0f;
		float rounding = 0.0f;
		glm::vec2 padding = { 0.0f, 0.0f };
	};

	struct Tab {
		Item background;
		Color border_color;
		Color text;
		Button tab_maximize_button;
		Button tab_minimize_button;
		Button node_maximize_button;
		Button node_minimize_button;
		float border = 0.0f;
		float rounding = 0.0f;
		float indent = 0.0f;
		glm::vec2 padding = { 0.0f, 0.0f };
		glm::vec2 spacing = { 0.0f, 0.0f };
	};

	struct Dropdown {
		Item normal;
		Item hover;
		Item active;
		Color border_color;
		Color label_normal;
		Color label_hover;
		Color label_active;
		Symbol symbol_normal;
		Symbol symbol_hover;
		Symbol symbol_active;
		Button button;
		float border = 0.0f;
		float rounding = 0.0f;
		glm::vec2 content_padding;
		glm::vec2 button_padding;
		glm::vec2 spacing;
	};

	struct Window {
		struct Header {
			Item normal;
			Item hover;
			Item active;
			Button close_button;
			Button minimize_button;
			Color label_normal;
			Color label_hover;
			Color label_active;
			glm::vec2 padding = { 0.0f, 0.0f };
			glm::vec2 label_padding = { 0.0f, 0.0f };
			glm::vec2 spacing = { 0.0f, 0.0f };
		};
		Header header;
		Item fixed_background;
		Color background;
		Color border_color;
		Color popup_border_color;
		Color dropdown_border_color;
		Color contextual_border_color;
		Color menu_border_color;
		Color group_border_color;
		Color tooltip_border_color;
		Item scaler;
		float border = 0.0f;
		float dropdown_border = 0.0f;
		float contextual_border = 0.0f;
		float menu_border = 0.0f;
		float group_border = 0.0f;
		float tooltip_border = 0.0f;
		float popup_border = 0.0f;
		float rounding = 0.0f;
		glm::vec2 spacing = { 0.0f, 0.0f };
		glm::vec2 group_padding = { 0.0f, 0.0f };
		glm::vec2 popup_padding = { 0.0f, 0.0f };
		glm::vec2 dropdown_padding;
		glm::vec2 contextual_padding;
		glm::vec2 menu_padding;
		glm::vec2 tooltip_padding;
	};

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
	Scrollbar scroll_horizontal;
	Scrollbar scroll_vertical;
	Tab tab;
	Dropdown dropdown;
	Window window;

	static Style from_mrb(mrb_state *mrb, mrb_value value);
	mrb_value to_mrb(mrb_state *mrb) const;
};

} /* namespace euler::gui */

#endif /* EULER_GUI_STYLE_H */
