/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_GUI_PROGRESS_BAR_H
#define EULER_GUI_PROGRESS_BAR_H

namespace euler::gui {
class ProgressBar : public Element {
public:
	ProgressBar(size_t max, bool modifiable);

private:
	size_t _max;
	bool _modifiable;
};
} /* namespace euler::gui */


#endif /* EULER_GUI_PROGRESS_BAR_H */

