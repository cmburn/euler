/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_INSTANCE_H
#define EULER_GUI_INSTANCE_H

#include "euler/util/object.h"

namespace euler::gui {

/*
 * Our GUI library is heavily patterned after
 * https://github.com/keharriso/love-nuklear/ but adapted to mesh better with
 * our engine architecture and mRuby. Both use Nuklear as the underlying GUI
 * library.
 */

/**
 * An Instance represents one GUI state. There should only be one Instance per
 * window.
 */
class Instance final : public util::Object {
public:
	Instance(const util::WeakReference<util::Logger> &parent);
};

} /* namespace euler::gui */


#endif /* EULER_GUI_INSTANCE_H */

