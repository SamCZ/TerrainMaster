#include "Renderer.h"
#include <iostream>
#include <memory>
#include "TerrainMaster/Utils/Buffers/Buffers.h"

namespace TM {

	Renderer::Renderer() : _activeRenderState() {
		_activeRenderState.BlendMode = BlendMode::Color;
		_activeRenderState.DepthTest = true;
		_activeRenderState.FaceCull = FaceCull::Back;
		_activeRenderState.LineWidth = 1.0f;
		_activeRenderState.Wireframe = false;

		//_firstStateInit = true;

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glLineWidth(1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glGenVertexArrays(1, &_vertexArrayId);

		glGetIntegerv(GL_DRAW_BUFFER, &_initialDrawBuf);
		glGetIntegerv(GL_READ_BUFFER, &_initialReadBuf);
		_activeShader = nullptr;

		_firstStateInit = true;
	}

	Renderer::~Renderer() {
		glDeleteVertexArrays(1, &_vertexArrayId);
	}

	void Renderer::ClearScreen(bool color, bool depth, bool stencil) {
		int flags = 0;
		if (color) flags |= GL_COLOR_BUFFER_BIT;
		if (depth) flags |= GL_DEPTH_BUFFER_BIT;
		if (stencil) flags |= GL_STENCIL_BUFFER_BIT;
		if (flags != 0) glClear(flags);
	}

	void Renderer::ClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void Renderer::ClearColor(ColorRGBA color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::SetShader(ShaderPtr shader) {
		if (_activeShader != shader | _invalidateShaderState) {
			_invalidateShaderState = false;
			_activeShader = shader;
			if (shader != nullptr) {
				if (!shader->IsCompiled()) {
					shader->Compile();
				}
				glUseProgram(shader->GetId());
			}
			else {
				glUseProgram(0);
			}
		}
	}

	void Renderer::ApplyRenderState(RenderState& state) {
		if (_activeRenderState.DepthTest != state.DepthTest || _firstStateInit) {
			if (state.DepthTest) {
				glEnable(GL_DEPTH_TEST);
			}
			else {
				glDisable(GL_DEPTH_TEST);
			}
			_activeRenderState.DepthTest = state.DepthTest;
		}

		if (_activeRenderState.BlendMode != state.BlendMode || _firstStateInit) {
			if (state.BlendMode == BlendMode::Off) {
				glDisable(GL_BLEND);
				//glDisable(GL_ALPHA_TEST);
			}
			else {
				glEnable(GL_BLEND);
				if (state.BlendMode == BlendMode::Additive) {
					glBlendFunc(GL_ONE, GL_ONE);
				}
				else if (state.BlendMode == BlendMode::Alpha) {
					glEnable(GL_ALPHA_TEST);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				}
				else if (state.BlendMode == BlendMode::AlphaAdditive) {
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				}
				else if (state.BlendMode == BlendMode::Color || state.BlendMode == BlendMode::Screen) {
					glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
				}
				else if (state.BlendMode == BlendMode::PremultAlpha) {
					glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				}
			}
			_activeRenderState.BlendMode = state.BlendMode;
		}

		if (_activeRenderState.FaceCull != state.FaceCull || _firstStateInit) {
			if (state.FaceCull == FaceCull::Off) {
				glDisable(GL_CULL_FACE);
			}
			else {
				glEnable(GL_CULL_FACE);
				if (state.FaceCull == FaceCull::Back) {
					glCullFace(GL_BACK);
				}
				else if (state.FaceCull == FaceCull::Front) {
					glCullFace(GL_FRONT);
				}
				else if (state.FaceCull == FaceCull::FrontAndBack) {
					glCullFace(GL_FRONT_AND_BACK);
				}
			}
			_activeRenderState.FaceCull = state.FaceCull;
		}

		if (_activeRenderState.LineWidth != state.LineWidth || _firstStateInit) {
			glLineWidth(state.LineWidth);
			_activeRenderState.LineWidth = state.LineWidth;
		}

		if (_activeRenderState.Wireframe != state.Wireframe || _firstStateInit) {
			if (state.Wireframe) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			_activeRenderState.Wireframe = state.Wireframe;
		}
		_firstStateInit = true;
	}

#pragma region "Render Mesh"
	void Renderer::RenderMesh(MeshPtr mesh, int lod) {
		if (_activeShader == nullptr) return;

		glBindVertexArray(_vertexArrayId);

		if (mesh->GetMode() == Mode::Patch) {
			glPatchParameteri(GL_PATCH_VERTICES, mesh->GetPatchVertexCount());
		}

		bool needsCompleteUpdate = false;

		if (mesh->_lastShader != _activeShader->GetId()) {
			needsCompleteUpdate = true;
			mesh->_enabledVertexAttribArrays = 0;
			mesh->_lastShader = _activeShader->GetId();
		}

		mesh->_enabledVertexAttribArrays = 0;
		for (int i = 0; i < 8; i++) {
			VertexBufferPtr vb = mesh->GetVertexBuffers()[i];
			if (vb == nullptr) continue;
			MeshBuffer::Type type = vb->GetType();
			if (type == MeshBuffer::Type::Index || vb->GetUsage() == MeshBuffer::Usage::CpuOnly) {
				continue;
			}
			if (!needsCompleteUpdate) {
				//if (!vb->isNeedsUpdate()) continue;
			}

			Attribute* attr = _activeShader->GetAttribute(type);
			if (attr->id == -1) {
				attr->id = _activeShader->GetAttributeLocation(attr->name);
				if (attr->id == -1) {
					continue;
				}
			}

			GLuint id = vb->GetId();

			int slotsRequired = 1;
			if (vb->GetNumComponents() > 4) {
				if (vb->GetNumComponents() % 4 != 0) {
					std::cerr << "Number of components in multi-slot buffers must be divisible by 4" << std::endl;
				}
				slotsRequired = vb->GetNumComponents() / 4;
			}

			if (vb->IsNeedsUpdate()) {
				UpdateBufferData(vb);
			}

			glBindBuffer(GL_ARRAY_BUFFER, vb->GetId());

			int slot = attr->id;

			if (slotsRequired == 1) {
				mesh->_vertexAttribArrays[mesh->_enabledVertexAttribArrays++] = slot;
				glVertexAttribPointer(attr->id, vb->GetNumComponents(), ConvertFormat(vb->GetFormat()), GL_FALSE, 0, (GLvoid*)0);
				glEnableVertexAttribArray(slot);
				glVertexAttribDivisorARB(attr->id, 0);
			}
			else {
				for (int i = 0; i < slotsRequired; i++) {
					glVertexAttribPointer(slot + i, 4, ConvertFormat(vb->GetFormat()), GL_FALSE, sizeof(float) * (4 * slotsRequired), (void*)(sizeof(float) * 4 * i));
					mesh->_vertexAttribArrays[mesh->_enabledVertexAttribArrays++] = slot + i;
					glEnableVertexAttribArray(slot + i);
					glVertexAttribDivisorARB(attr->id + i, 1);
				}
			}
		}
		FinishRenderMesh(mesh);
	}

	void Renderer::FinishRenderMesh(MeshPtr mesh) {
		VertexBufferPtr ib = mesh->GetVertexBuffer(MeshBuffer::Type::Index);
		VertexBufferPtr pb = mesh->GetVertexBuffer(MeshBuffer::Type::Position);

		for (int i = 0; i < mesh->_enabledVertexAttribArrays; i++) {
			glEnableVertexAttribArray(mesh->_vertexAttribArrays[i]);
		}

		if (ib != nullptr) {
			if (ib->IsNeedsUpdate()) {
				UpdateBufferData(ib);
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->GetId());
			if (mesh->GetInstanceCount() > 1) {
				glDrawElementsInstanced((GLenum)mesh->GetMode(), static_cast<GLsizei>(ib->GetBuffer()->Size()), ConvertFormat(ib->GetFormat()), 0, mesh->GetInstanceCount());
			}
			else {
				glDrawElements((GLenum)mesh->GetMode(), static_cast<GLsizei>(ib->GetBuffer()->Size()), ConvertFormat(ib->GetFormat()), 0);
			}
		}
		else if (pb != nullptr) {
			if (mesh->GetInstanceCount() > 1) {
				glDrawArraysInstanced((GLenum)mesh->GetMode(), 0, static_cast<GLsizei>(pb->GetBuffer()->Size() / pb->GetNumComponents()), mesh->GetInstanceCount());
			}
			else {
				glDrawArrays((GLenum)mesh->GetMode(), 0, static_cast<GLsizei>(pb->GetBuffer()->Size() / pb->GetNumComponents()));
			}
		}
		for (int i = 0; i < mesh->_enabledVertexAttribArrays; i++) {
			glDisableVertexAttribArray(mesh->_vertexAttribArrays[i]);
		}
	}

	void Renderer::UpdateBufferData(VertexBufferPtr vb) {
		GLuint id = vb->GetId();
		if (id == -1) {
			glGenBuffers(1, &id);
			vb->SetId(id);
		}

		int target = GL_ARRAY_BUFFER;
		if (vb->GetType() == MeshBuffer::Type::Index) {
			target = GL_ELEMENT_ARRAY_BUFFER;
		}
		glBindBuffer(target, id);

		GLenum usage = ConvertUsage(vb->GetUsage());
		BufferPtr buffer = vb->GetBuffer();
		if (buffer == nullptr) {
			std::cerr << "Buffer cannot be null !" << std::endl;
		}
		switch (buffer->GetType()) {
		case BufferType::Float:
		{
			FloatBufferPtr fb = std::dynamic_pointer_cast<FloatBuffer>(buffer);
			glBufferData(target, fb->Size() * sizeof(float), fb->GetData(), usage);
		}
		break;
		case BufferType::Int:
		{
			IntBufferPtr ib = std::dynamic_pointer_cast<IntBuffer>(buffer);
			glBufferData(target, ib->Size() * sizeof(unsigned int), ib->GetData(), usage);
		}
		break;
		}

		vb->MarkClean();
	}

	GLenum Renderer::ConvertUsage(MeshBuffer::Usage usage) {
		switch (usage) {
		case MeshBuffer::Usage::Static:
			return GL_STATIC_DRAW;
		case MeshBuffer::Usage::Dynamic:
			return GL_DYNAMIC_DRAW;
		case MeshBuffer::Usage::Stream:
			return GL_STREAM_DRAW;
		default:
			return GL_NONE;
		}
	}

	GLenum Renderer::ConvertFormat(MeshBuffer::Format format) {
		switch (format) {
		case MeshBuffer::Format::Float:
			return GL_FLOAT;
		case MeshBuffer::Format::Int:
			return GL_UNSIGNED_INT;
		default:
			return GL_NONE;
		}
	}
#pragma endregion

}