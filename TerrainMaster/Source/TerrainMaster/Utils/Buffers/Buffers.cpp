#include "Buffers.h"

namespace TM {
	FloatBufferPtr CreateFloatBuffer(float * data, size_t size) {
		FloatBufferPtr fb = FloatBuffer::Allocate(size);
		fb->Put(data, size);
		delete[] data;
		return fb;
	}
	IntBufferPtr CreateIntBuffer(int * data, size_t size) {
		IntBufferPtr ib = IntBuffer::Allocate(size);
		ib->Put(data, size);
		delete[] data;
		return ib;
	}

	FloatBufferPtr CreateFloatBuffer(std::vector<glm::vec3>& data) {
		FloatBufferPtr fb = FloatBuffer::Allocate(data.size() * 3);
		for (glm::vec3 v : data) {
			fb->Put(v.x)->Put(v.y)->Put(v.z);
		}
		return fb;
	}

	IntBufferPtr CreateIntBuffer(std::vector<int>& data) {
		IntBufferPtr ib = IntBuffer::Allocate(data.size());
		ib->Put(&data[0], data.size());
		return ib;
	}

	void PopulateFromBuffer(glm::vec3& vector, IntBufferPtr buf, int index) {
		vector.x = static_cast<float>(buf->Get(index * 3));
		vector.y = static_cast<float>(buf->Get(index * 3 + 1));
		vector.z = static_cast<float>(buf->Get(index * 3 + 2));
	}

	void PopulateFromBuffer2(glm::vec2& vector, IntBufferPtr buf, int index) {
		vector.x = static_cast<float>(buf->Get(index * 2));
		vector.y = static_cast<float>(buf->Get(index * 2 + 1));
	}
}