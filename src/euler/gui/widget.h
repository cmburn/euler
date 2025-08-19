/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_WIDGET_H
#define EULER_GUI_WIDGET_H

#include <optional>
#include <string>

#include <glm/glm.hpp>

#include "element.h"
#include "euler/util/object.h"

namespace euler::gui {
class Widget : public util::Object {
public:
	Widget(const util::WeakReference<util::Logger> &parent);
	using Rectangle = glm::mat2;

	struct Config {
		Config();
		std::optional<std::string> title;
		bool draw_border : 1;
		bool is_movable : 1;
		bool is_scalable : 1;
		bool is_closable : 1;
		bool is_minimizable : 1;
		bool has_scrollbar : 1;
		bool auto_hide_scrollbar : 1;
		bool keep_in_background : 1;
		bool scaler_on_left : 1;
		bool no_input : 1;
	};

	const Config &config() const;
	void set_config(const Config &config);
	const std::string &name() const;

	void
	set_position(const float x, const float y)
	{
		_position = glm::vec2(x, y);
	}

	void
	set_size(const float w, const float h)
	{
		_size = glm::vec2(w, h);
	}

	Widget(std::string_view name, float x, float y, float w, float h,
	    Config config = {});

	void
	add_row(std::vector<util::Reference<Element>> &&row)
	{
		_rows.emplace_back(std::move(row));
	}

private:
	Config _config;
	std::string _name;
	glm::vec2 _position;
	glm::vec2 _size;
	std::vector<std::vector<util::Reference<Element>>> _rows;

	bool _first_init;
};
} /* namespace euler::gui */


#endif /* EULER_GUI_WIDGET_H */

