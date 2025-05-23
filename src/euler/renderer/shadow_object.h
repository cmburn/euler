/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_SHADOW_OBJECT_H
#define EULER_RENDERER_SHADOW_OBJECT_H

#include "Eigen/Core"

#include "euler/util/object.h"

namespace Euler::Renderer {
class ShadowEnvironment;

/* TODO: Turn WeakReference<ShadowEnvironment> into Reference<> when
 *       shared with user */

class ShadowObject final : public Util::Object {
	friend class ShadowEnvironment;

public:
	using ValueType = int32_t;
private:
	ShadowObject(Util::WeakReference<ShadowEnvironment>, ValueType value);
public:
	ValueType
	value() const
	{
		return _value;
	}
	void set_position(Eigen::Vector2f position);
	void update(Eigen::Vector2f position, Eigen::Vector2f scale,
	    Eigen::Vector2f origin, float angle);

	void set_visible(bool visible);
	bool visible() const;

private:
	ValueType _value = 0;
	Util::WeakReference<ShadowEnvironment> _environment;
};

} /* namespace Euler::Renderer */

#endif /* EULER_RENDERER_SHADOW_OBJECT_H */

