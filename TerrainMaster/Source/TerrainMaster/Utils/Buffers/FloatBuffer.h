#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Buffer.h"

namespace TM {

	class FloatBuffer;

	typedef SharedPtr<FloatBuffer> FloatBufferPtr;

	class FloatBuffer : public Buffer, public std::enable_shared_from_this<FloatBuffer> {
	private:
		size_t _size;
		float* _data;
	public:
		FloatBuffer(size_t size);
		~FloatBuffer();
		static FloatBufferPtr Allocate(size_t size);
		static FloatBufferPtr Allocate3(std::vector<glm::vec3> vec);
		static FloatBufferPtr Allocate2(std::vector<glm::vec2> vec);

		inline size_t Size() override {
			return _size;
		}
		void Clear() override;

		inline BufferType GetType() override {
			return BufferType::Float;
		}

		FloatBufferPtr Put(float f);
		FloatBufferPtr Put(int index, float f);

		FloatBufferPtr Put(float* data, size_t size);

		//void put(float* data, int size);

		float Get();
		float Get(int index);

		float* GetData();

		void Print(int c);
	};
}
