#include "FloatBuffer.h"
#include <iostream>

namespace TM {
	FloatBuffer::FloatBuffer(size_t size) {
		_data = new float[size];
		_size = size;
		_nextIndex = 0;
	}

	FloatBuffer::~FloatBuffer() {
		delete[] _data;
	}

	FloatBufferPtr FloatBuffer::Allocate(size_t size) {
		return MakeShared<FloatBuffer>(size);
	}

	FloatBufferPtr FloatBuffer::Allocate3(std::vector<glm::vec3> vec) {
		FloatBufferPtr buff = FloatBuffer::Allocate(vec.size() * 3);
		for (glm::vec3& v : vec) {
			buff->Put(v.x)->Put(v.y)->Put(v.z);
		}
		return buff;
	}

	FloatBufferPtr FloatBuffer::Allocate2(std::vector<glm::vec2> vec) {
		FloatBufferPtr buff = FloatBuffer::Allocate(vec.size() * 2);
		for (glm::vec2& v : vec) {
			buff->Put(v.x)->Put(v.y);
		}
		return buff;
	}

	void FloatBuffer::Clear() {
		memset(_data, 0, _size * sizeof(float));
		Rewind();
	}

	FloatBufferPtr FloatBuffer::Put(float f) {
		int index = _nextIndex++;
		if (index > _size - 1) {
			std::cerr << "Buffer overflow exception ! (" << index << ")" << std::endl;
			return shared_from_this();
		}
		_data[index] = f;
		return shared_from_this();
	}

	FloatBufferPtr FloatBuffer::Put(int index, float f) {
		if (index > _size - 1) {
			std::cerr << "Buffer overflow exception ! (" << index << ")" << std::endl;
			return shared_from_this();
		}
		_data[index] = f;
		return shared_from_this();
	}

	FloatBufferPtr FloatBuffer::Put(float* data, size_t size) {
		for (int i = 0; i < size; i++) {
			Put(data[i]);
		}
		return shared_from_this();
	}

	float FloatBuffer::Get() {
		return _data[_nextIndex];
	}

	float FloatBuffer::Get(int index) {
		return _data[index];
	}

	float* FloatBuffer::GetData() {
		return _data;
	}
	void FloatBuffer::Print(int c) {
		for (int i = 0; i < Size() / c; i++) {
			for (int a = 0; a < c; a++) {
				std::cout << Get(i * c + a) << ",";
			}
			std::cout << std::endl;
		}
	}
}