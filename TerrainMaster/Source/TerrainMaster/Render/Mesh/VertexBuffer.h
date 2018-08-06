#pragma once

#include <memory>

#include "TerrainMaster/Utils/NativeObject.h"
#include "TerrainMaster/Utils/Buffers/Buffers.h"

namespace TM {
	namespace MeshBuffer {
		enum class Type {
			Position,
			Normal,
			TexCoord,
			Color,
			Tangent,
			BiTangent,
			Index,
			InstanceData
		};

		enum class Usage {
			Static,
			Dynamic,
			Stream,
			CpuOnly
		};

		enum class Format {
			Float,
			Int
		};
	}

	class VertexBuffer : public NativeObject {
	public:
		VertexBuffer(MeshBuffer::Type type);
		~VertexBuffer();

		void SetupData(MeshBuffer::Usage usage, int components, MeshBuffer::Format format, BufferPtr buffer);
		void UpdateData(BufferPtr buffer);

		void SetUsage(MeshBuffer::Usage usage);

		MeshBuffer::Type GetType() const;
		MeshBuffer::Usage GetUsage() const;
		MeshBuffer::Format GetFormat() const;

		int GetNumComponents() const;
		BufferPtr GetBuffer();
	private:
		MeshBuffer::Type _type;
		MeshBuffer::Usage _usage;
		MeshBuffer::Format _format;
		int _components;
		BufferPtr _buffer;
	};

	typedef SharedPtr<VertexBuffer> VertexBufferPtr;
}
