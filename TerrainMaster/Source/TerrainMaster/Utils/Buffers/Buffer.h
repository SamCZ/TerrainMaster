#pragma once

#include "../SharedPtr.h"

namespace TM {

	enum class BufferType {
		Float,
		Int,
		Byte,
		Short
	};

	class Buffer {
	public:
		virtual inline ~Buffer() {  }
		virtual size_t Size() = 0;
		virtual void Clear() = 0;
		virtual BufferType GetType() = 0;

		inline void Rewind() {
			_nextIndex = 0;
		}

		inline void Position(int pos) {
			_nextIndex = pos;
		}

	protected:
		int _nextIndex = 0;
	};

	typedef SharedPtr<Buffer> BufferPtr;

}