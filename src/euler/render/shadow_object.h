/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_RENDER_SHADOW_OBJECT_H
#define EULER_RENDER_SHADOW_OBJECT_H

namespace euler::render {
class ShadowEnvironment;

class ShadowObject final : public util::Object {
	friend class ShadowEnvironment;

public:
	using ValueType = int32_t;
private:
	ShadowObject(util::WeakReference<ShadowEnvironment>, ValueType value);
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

	util::Reference<ShadowEnvironment> environment() const;

private:
	ValueType _value = 0;
	util::WeakReference<ShadowEnvironment> _environment;
};

} /* namespace euler::render */


#endif /* EULER_RENDER_SHADOW_OBJECT_H */

