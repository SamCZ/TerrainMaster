#include "Mesh.h"

namespace TM {
	Mesh::Mesh() : _vertexBuffersArr(new VertexBufferPtr[8]) {
		_mode = Mode::Triangles;
		_patchVertexCount = 0;
		_enabledVertexAttribArrays = 0;
		for (int i = 0; i < 8; i++) {
			_vertexBuffersArr[i] = nullptr;
		}
	}

	Mesh::~Mesh() {
		for (int i = 0; i < 8; i++) {
			if (_vertexBuffersArr[i]) {
				//delete _vertexBuffersArr[i];
			}
		}
		delete[] _vertexBuffersArr;
	}

	void Mesh::SetData(MeshBuffer::Type type, int components, BufferPtr buffer) {
		if (_vertexBuffersArr[(int)type] != nullptr) {
			_vertexBuffersArr[(int)type]->UpdateData(buffer);
		}
		else {
			VertexBufferPtr vb = MakeShared<VertexBuffer>(type);
			MeshBuffer::Format format;
			switch (buffer->GetType()) {
			case BufferType::Float:
				format = MeshBuffer::Format::Float;
				break;
			case BufferType::Int:
				format = MeshBuffer::Format::Int;
				break;
			}
			vb->SetupData(MeshBuffer::Usage::Dynamic, components, format, buffer);
			_vertexBuffersArr[(int)type] = vb;
		}
	}

	void Mesh::SetVertexBuffer(MeshBuffer::Type type, VertexBufferPtr buffer) {
		_vertexBuffersArr[(int)type] = buffer;
	}

	VertexBufferPtr Mesh::GetVertexBuffer(MeshBuffer::Type type) {
		return _vertexBuffersArr[(int)type];
	}

	VertexBufferPtr* Mesh::GetVertexBuffers() {
		return _vertexBuffersArr;
	}

	Mode Mesh::GetMode() const {
		return _mode;
	}
	void Mesh::SetMode(Mode mode) {
		_mode = mode;
	}
	/*
	void Mesh::createCollisionData() {
		BIHTree* ct = new BIHTree(this);
		ct->construct();
		_collisionTree = ct;
	}

	BIHTree* Mesh::getCollider() {
		return _collisionTree;
	}

	BoundingBox* Mesh::getBounds() {
		return _boundingBox;
	}

	void Mesh::setBounds(BoundingBox * bound) {
		_boundingBox = bound;
	}

	void Mesh::updateBounds() {
		if (_boundingBox == nullptr) {
			_boundingBox = new BoundingBox();
		}
		_boundingBox->computeFromMesh(this);
	}*/

	int Mesh::GetPatchVertexCount() const {
		return _patchVertexCount;
	}

	void Mesh::SetPatchVertexCount(int patchCount) {
		_patchVertexCount = patchCount;
	}
	int Mesh::GetInstanceCount() const {
		return _instanceCount;
	}
	void Mesh::SetInstanceCount(int count) {
		_instanceCount = count;
	}
}