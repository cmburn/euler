/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_SHADOW_ENVIRONMENT_H
#define EULER_VULKAN_SHADOW_ENVIRONMENT_H

#include <semaphore>

#include <glm/glm.hpp>

#include "euler/util/object.h"

namespace euler::vulkan {
class ShadowEnvironment final {
public:
	class Object final {
		friend class ShadowEnvironment;
	public:
		using Index = int32_t;

		struct Position {
			glm::vec2 position;
			glm::vec2 scale;
			glm::vec2 origin;
			float theta;
		};

		void add_edge(glm::vec2 start, glm::vec2 end);
		void set_position(glm::vec2 position);

		~Object();

	private:
		util::WeakReference<ShadowEnvironment> _environment;
		std::binary_semaphore _alive { 1 };
		int32_t _index = 0;
		bool _visible = true;
	};

	util::Reference<Object> add_object();
	void reset_edges();
	void flush_vbo();


private:
	friend class Object;
	std::unordered_map<Object::Index, Object> _objects;
};
} /* namespace euler::vulkan */


#endif /* EULER_VULKAN_SHADOW_ENVIRONMENT_H */

