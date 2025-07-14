/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_RENDER_SHADOW_ENVIRONMENT_H
#define EULER_RENDER_SHADOW_ENVIRONMENT_H

namespace euler::render {
class ShadowEnvironment final : public util::Object {
	friend class ShadowObject;
public:

	ShadowEnvironment();
	~ShadowEnvironment() override;

	util::Reference<ShadowObject> add_object();
	void add_edge(Eigen::Vector2f start, Eigen::Vector2f end);
	void reset_edges();
	void flush_vbo();

private:
	VK2DShadowEnvironment _shadow_environment;
	std::unordered_map<ShadowObject::ValueType, ShadowObject> _objects;
};
} /* namespace euler::render */


#endif /* EULER_RENDER_SHADOW_ENVIRONMENT_H */

