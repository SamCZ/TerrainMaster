#pragma once

#include "FloatBuffer.h"
#include "IntBuffer.h"

namespace TM {
	FloatBufferPtr CreateFloatBuffer(float* data, size_t size);
	IntBufferPtr CreateIntBuffer(int* data, size_t size);

	void PopulateFromBuffer(glm::vec3& vector, IntBufferPtr buf, int index);
	void PopulateFromBuffer2(glm::vec2& vector, IntBufferPtr buf, int index);
}