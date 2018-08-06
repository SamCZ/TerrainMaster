#pragma once

#include <vector>
#include "Buffer.h"

namespace TM {

	class IntBuffer;

	typedef SharedPtr<IntBuffer> IntBufferPtr;

	class IntBuffer : public Buffer, public std::enable_shared_from_this<IntBuffer> {
	private:
		size_t _size;
		int* _data;
	public:
		IntBuffer(size_t size);
		~IntBuffer();

		inline size_t Size() override {
			return _size;
		}
		void Clear() override;

		inline BufferType GetType() override {
			return BufferType::Int;
		}

		IntBufferPtr Put(int i);
		IntBufferPtr Put(int index, int i);
		IntBufferPtr Put(int* data, size_t size);

		int Get();
		int Get(int index);

		int* GetData();

		void Print(int c);

		static IntBufferPtr Allocate(size_t size);
		static IntBufferPtr Allocate(std::vector<unsigned int> vec);
	};
}