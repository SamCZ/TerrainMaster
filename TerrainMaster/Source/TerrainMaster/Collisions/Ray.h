#pragma once

#include <glm/glm.hpp>

namespace TM {
	class Ray {
	public:
		glm::vec3 Origin;
		glm::vec3 Direction;
		float Limit;

		Ray();
		Ray(glm::vec3 origin, glm::vec3 dir);

		float Intersects(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
	};
}