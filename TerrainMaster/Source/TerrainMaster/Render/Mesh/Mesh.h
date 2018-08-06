#pragma once

#include <map>
#include "VertexBuffer.h"
#include "TerrainMaster/Utils/SharedPtr.h"

namespace TM {

	enum class Mode : GLenum {
		Triangles = GL_TRIANGLES,
		Quads = GL_QUADS,
		TriangleTrip = GL_TRIANGLE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN,
		Lines = GL_LINES,
		Points = GL_POINTS,
		Patch = GL_PATCHES
	};

	class Mesh {
	private:
		Mode _mode;
		VertexBufferPtr* _vertexBuffersArr;
		/*BIHTree* _collisionTree = nullptr;
		BoundingBox* _boundingBox = nullptr;*/
		int _patchVertexCount;
		int _instanceCount;
	public:
		GLuint _vertexAttribArrays[16];
		int _enabledVertexAttribArrays = 0;
		GLuint _lastShader;

		Mesh();
		~Mesh();

		void SetData(MeshBuffer::Type type, int components, BufferPtr buffer);
		void SetData(MeshBuffer::Type type, int components, FloatBufferPtr buffer);
		void SetData(MeshBuffer::Type type, int components, IntBufferPtr buffer);

		void SetVertexBuffer(MeshBuffer::Type type, VertexBufferPtr buffer);
		VertexBufferPtr GetVertexBuffer(MeshBuffer::Type type);

		VertexBufferPtr* GetVertexBuffers();

		Mode GetMode() const;
		void SetMode(Mode mode);

		/*void createCollisionData();
		BIHTree* getCollider();*/

		/*BoundingBox* getBounds();
		void setBounds(BoundingBox* bound);
		void updateBounds();*/

		int GetPatchVertexCount() const;
		void SetPatchVertexCount(int patchCount);

		int GetInstanceCount() const;
		void SetInstanceCount(int count);
	};

	typedef SharedPtr<Mesh> MeshPtr;
}