#include "Ray.h"
#include <limits>

namespace TM {
	Ray::Ray() : Origin(), Direction(0, 0, 1) {
		Limit = std::numeric_limits<float>::max();
	}

	Ray::Ray(glm::vec3 origin, glm::vec3 dir) : Origin(origin), Direction(dir) {
		Limit = std::numeric_limits<float>::max();
	}

	float Ray::Intersects(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
		float edge1X = v1.x - v0.x;
		float edge1Y = v1.y - v0.y;
		float edge1Z = v1.z - v0.z;

		float edge2X = v2.x - v0.x;
		float edge2Y = v2.y - v0.y;
		float edge2Z = v2.z - v0.z;

		float normX = ((edge1Y * edge2Z) - (edge1Z * edge2Y));
		float normY = ((edge1Z * edge2X) - (edge1X * edge2Z));
		float normZ = ((edge1X * edge2Y) - (edge1Y * edge2X));

		float dirDotNorm = Direction.x * normX + Direction.y * normY + Direction.z * normZ;

		float diffX = Origin.x - v0.x;
		float diffY = Origin.y - v0.y;
		float diffZ = Origin.z - v0.z;

		float sign;
		if (dirDotNorm > FLT_EPSILON) {
			sign = 1;
		}
		else if (dirDotNorm < -FLT_EPSILON) {
			sign = -1.0f;
			dirDotNorm = -dirDotNorm;
		}
		else {
			// ray and triangle/quad are parallel
			return std::numeric_limits<float>::infinity();
		}

		float diffEdge2X = ((diffY * edge2Z) - (diffZ * edge2Y));
		float diffEdge2Y = ((diffZ * edge2X) - (diffX * edge2Z));
		float diffEdge2Z = ((diffX * edge2Y) - (diffY * edge2X));

		float dirDotDiffxEdge2 = sign * (Direction.x * diffEdge2X
			+ Direction.y * diffEdge2Y
			+ Direction.z * diffEdge2Z);

		if (dirDotDiffxEdge2 >= 0.0f) {
			diffEdge2X = ((edge1Y * diffZ) - (edge1Z * diffY));
			diffEdge2Y = ((edge1Z * diffX) - (edge1X * diffZ));
			diffEdge2Z = ((edge1X * diffY) - (edge1Y * diffX));

			float dirDotEdge1xDiff = sign * (Direction.x * diffEdge2X
				+ Direction.y * diffEdge2Y
				+ Direction.z * diffEdge2Z);

			if (dirDotEdge1xDiff >= 0.0f) {
				if (dirDotDiffxEdge2 + dirDotEdge1xDiff <= dirDotNorm) {
					float diffDotNorm = -sign * (diffX * normX + diffY * normY + diffZ * normZ);
					if (diffDotNorm >= 0.0f) {
						// ray intersects triangle
						// fill in.
						float inv = 1.0f / dirDotNorm;
						float t = diffDotNorm * inv;
						return t;
					}
				}
			}
		}

		return std::numeric_limits<float>::infinity();
	}
}