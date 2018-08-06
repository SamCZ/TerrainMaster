#include "IntBuffer.h"
#include <iostream>

namespace TM {
	IntBuffer::IntBuffer(size_t size) {
		_data = new int[size];
		_size = size;
	}

	IntBuffer::~IntBuffer() {
		delete[] _data;
	}

	IntBufferPtr IntBuffer::Allocate(size_t size) {
		return MakeShared<IntBuffer>(size);
	}

	IntBufferPtr IntBuffer::Allocate(std::vector<unsigned int> vec) {
		IntBufferPtr buff = IntBuffer::Allocate(vec.size());
		for (unsigned int& v : vec) {
			buff->Put(v);
		}
		return buff;
	}

	void IntBuffer::Clear() {
		memset(_data, 0, _size * sizeof(float));
		Rewind();
	}

	IntBufferPtr IntBuffer::Put(int i) {
		int index = _nextIndex++;
		if (index > _size - 1) {
			std::cerr << "Buffer overflow exception ! (" << index << ")" << std::endl;
			return shared_from_this();
		}
		_data[index] = i;
		return shared_from_this();
	}

	IntBufferPtr IntBuffer::Put(int index, int i) {
		if (index > _size - 1) {
			std::cerr << "Buffer overflow exception ! (" << index << ")" << std::endl;
			return shared_from_this();
		}
		_data[index] = i;
		return shared_from_this();
	}

	IntBufferPtr IntBuffer::Put(int* data, size_t size) {
		for (int i = 0; i < size; i++) {
			Put(data[i]);
		}
		return shared_from_this();
	}

	int IntBuffer::Get() {
		return _data[_nextIndex];
	}

	int IntBuffer::Get(int index) {
		return _data[index];
	}

	int* IntBuffer::GetData() {
		return _data;
	}
	void IntBuffer::Print(int c) {
		for (int i = 0; i < Size() / c; i++) {
			for (int a = 0; a < c; a++) {
				std::cout << Get(i * c + a) << ",";
			}
			std::cout << std::endl;
		}
	}
}