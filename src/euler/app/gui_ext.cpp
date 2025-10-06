/* SPDX-License-Identifier: ISC */

#include "euler/app/gui_ext.h"

#include "euler/app/ext.h"
#include "euler/gui/button.h"

#include <mruby/hash.h>
#include <mruby/class.h>

using namespace euler;
using Modules = app::State::Modules;

static gui::Symbol
to_symbol(mrb_sym sym)
{
	switch (sym) {
	case MRB_SYM(none): return gui::Symbol::None;
	case MRB_SYM(x): return gui::Symbol::X;
	case MRB_SYM(underscore): return gui::Symbol::Underscore;
	case MRB_SYM(solid_circle): return gui::Symbol::SolidCircle;
	case MRB_SYM(outlined_circle): return gui::Symbol::OutlinedCircle;
	case MRB_SYM(solid_rectangle): return gui::Symbol::SolidRectangle;
	case MRB_SYM(outlined_rectangle): return gui::Symbol::OutlinedRectangle;
	case MRB_SYM(up_triangle): return gui::Symbol::UpTriangle;
	case MRB_SYM(down_triangle): return gui::Symbol::DownTriangle;
	case MRB_SYM(left_triangle): return gui::Symbol::LeftTriangle;
	case MRB_SYM(right_triangle): return gui::Symbol::RightTriangle;
	case MRB_SYM(plus): return gui::Symbol::Plus;
	case MRB_SYM(minus): return gui::Symbol::Minus;
	case MRB_SYM(outlined_up_triangle):
		return gui::Symbol::OutlinedUpTriangle;
	case MRB_SYM(outlined_down_triangle):
		return gui::Symbol::OutlinedDownTriangle;
	case MRB_SYM(outlined_left_triangle):
		return gui::Symbol::OutlinedLeftTriangle;
	case MRB_SYM(outlined_right_triangle):
		return gui::Symbol::OutlinedRightTriangle;
	default: throw std::invalid_argument("Invalid symbol");
	}
}

static mrb_value
from_symbol(gui::Symbol symbol)
{
	switch (symbol) {
	case gui::Symbol::None: return mrb_symbol_value(MRB_SYM(none));
	case gui::Symbol::X: return mrb_symbol_value(MRB_SYM(x));
	case gui::Symbol::Underscore:
		return mrb_symbol_value(MRB_SYM(underscore));
	case gui::Symbol::SolidCircle:
		return mrb_symbol_value(MRB_SYM(solid_circle));
	case gui::Symbol::OutlinedCircle:
		return mrb_symbol_value(MRB_SYM(outlined_circle));
	case gui::Symbol::SolidRectangle:
		return mrb_symbol_value(MRB_SYM(solid_rectangle));
	case gui::Symbol::OutlinedRectangle:
		return mrb_symbol_value(MRB_SYM(outlined_rectangle));
	case gui::Symbol::UpTriangle:
		return mrb_symbol_value(MRB_SYM(up_triangle));
	case gui::Symbol::DownTriangle:
		return mrb_symbol_value(MRB_SYM(down_triangle));
	case gui::Symbol::LeftTriangle:
		return mrb_symbol_value(MRB_SYM(left_triangle));
	case gui::Symbol::RightTriangle:
		return mrb_symbol_value(MRB_SYM(right_triangle));
	case gui::Symbol::Plus: return mrb_symbol_value(MRB_SYM(plus));
	case gui::Symbol::Minus: return mrb_symbol_value(MRB_SYM(minus));
	case gui::Symbol::OutlinedUpTriangle:
		return mrb_symbol_value(MRB_SYM(outlined_up_triangle));
	case gui::Symbol::OutlinedDownTriangle:
		return mrb_symbol_value(MRB_SYM(outlined_down_triangle));
	case gui::Symbol::OutlinedLeftTriangle:
		return mrb_symbol_value(MRB_SYM(outlined_left_triangle));
	case gui::Symbol::OutlinedRightTriangle:
		return mrb_symbol_value(MRB_SYM(outlined_right_triangle));
	default: throw std::invalid_argument("Invalid symbol");
	}
}

extern const mrb_data_type app::GUI_BUTTON_TYPE
    = MAKE_REFERENCE_TYPE(euler::gui::Button);

static mrb_value
button_symbol(mrb_state *mrb, const mrb_value self_value)
{
	const auto self = app::unwrap<gui::Button>(mrb, self_value,
	    &app::GUI_BUTTON_TYPE);
	return from_symbol(self->symbol());
}

static void
init_button(mrb_state *mrb, Modules &mod)
{
	mod.gui.button = mrb_define_class_under(mrb, mod.gui.module, "Button",
	    mod.gui.element);
	//const auto button = mod.gui.button;
	// MRB_SET_INSTANCE_TT(mod.gui.button, MRB_TT_CDATA);
}

void
app::init_gui(util::Reference<State> state)
{
	state->log()->info("Initializing Euler::GUI...");
	auto mrb = state->mrb();
	auto &mod = state->module();
	mod.gui.module = mrb_define_module_under(mrb, mod.module, "GUI");
	state->log()->info("Euler::GUI initialized");
}