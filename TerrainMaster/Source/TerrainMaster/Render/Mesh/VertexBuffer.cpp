#include "VertexBuffer.h"
#include <iostream>

namespace TM {
	VertexBuffer::VertexBuffer(MeshBuffer::Type type) {
		_type = type;
		_buffer = nullptr;
	}

	VertexBuffer::~VertexBuffer() {
		/*try {
			if (_buffer) {
				delete _buffer;
				_buffer = NULL;
			}
		}
		catch (int e) {

		}*/
		
		if (_id != -1) {
			glDeleteBuffers(1, &_id);
		}
	}

	void VertexBuffer::SetupData(MeshBuffer::Usage usage, int components, MeshBuffer::Format format, BufferPtr buffer) {
		if (_id != -1) {
			std::cerr << "Data has already been sent. Cannot setupData again." << std::endl;
			return;
		}
		_usage = usage;
		_components = components;
		_format = format;
		_buffer = buffer;
		MarkUpdate();
	}

	void VertexBuffer::UpdateData(BufferPtr buffer) {
		_buffer = buffer;
		MarkUpdate();
	}

	void VertexBuffer::SetUsage(MeshBuffer::Usage usage) {
		_usage = usage;
		MarkUpdate();
	}

	MeshBuffer::Type VertexBuffer::GetType() const {
		return _type;
	}

	MeshBuffer::Usage VertexBuffer::GetUsage() const {
		return _usage;
	}

	MeshBuffer::Format VertexBuffer::GetFormat() const {
		return _format;
	}

	int VertexBuffer::GetNumComponents() const {
		return _components;
	}

	BufferPtr VertexBuffer::GetBuffer() {
		return _buffer;
	}
}