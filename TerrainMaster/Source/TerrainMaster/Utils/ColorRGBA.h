#pragma once

#include <glm/glm.hpp>

namespace TM {
	struct ColorRGBA {

		float r;
		float g;
		float b;
		float a;

		inline glm::vec4 ToVec4() const {
			return glm::vec4(r, g, b, a);
		}
		inline glm::vec3 ToVec3() const {
			return glm::vec3(r, g, b);
		}
	};

	ColorRGBA MakeRGB(int r, int g, int b);
	ColorRGBA MakeRGBA(int r, int g, int b, int a);
	ColorRGBA MakeRGBf(float r, float g, float b);
	ColorRGBA MakeRGBAf(float r, float g, float b, float a);
	ColorRGBA MakeHEX(const char* hex);
}