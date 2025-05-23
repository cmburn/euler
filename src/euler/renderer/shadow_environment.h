/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_SHADOW_ENVIRONMENT_H
#define EULER_RENDERER_SHADOW_ENVIRONMENT_H

#include <unordered_map>

#include "Eigen/Core"

#include "euler/renderer/shadow_object.h"
#include "euler/util/object.h"

/* ReSharper disable once CppRedundantElaboratedTypeSpecifier */
typedef struct VK2DShadowEnvironment_t *VK2DShadowEnvironment;

namespace Euler::Renderer {
class ShadowEnvironment;

class ShadowEnvironment final : public Util::Object {
public:

private:
	friend class ShadowObject;
public:

	ShadowEnvironment();
	~ShadowEnvironment() override;

	Util::Reference<ShadowObject> add_object();
	void add_edge(Eigen::Vector2f start, Eigen::Vector2f end);
	void reset_edges();
	void flush_vbo();

private:

	VK2DShadowEnvironment _shadow_environment;
	std::unordered_map<ShadowObject::ValueType, ShadowObject> _objects;
};
} /* namespace Euler::Renderer */

#endif /* EULER_RENDERER_SHADOW_ENVIRONMENT_H */
