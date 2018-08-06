#pragma once

#include "TerrainMaster/Utils/ColorRGBA.h"
#include "RenderState.h"
#include "Shader.h"

namespace TM {

	class Renderer {
	private:
		ShaderPtr _activeShader = nullptr;
		GLuint _vertexArrayId;
		RenderState _activeRenderState;

		GLint _initialDrawBuf;
		GLint _initialReadBuf;

		bool _invalidateShaderState;
		bool _firstStateInit;
	public:
		Renderer();
		~Renderer();

		void ClearScreen(bool color, bool depth, bool stencil);
		void ClearColor(float r, float g, float b, float a);
		void ClearColor(ColorRGBA color);

		void SetShader(ShaderPtr shader);

		void ApplyRenderState(RenderState& state);

		void RenderMesh(MeshPtr mesh, int lod);
		void FinishRenderMesh(MeshPtr mesh);

	private:
		void UpdateBufferData(VertexBufferPtr vb);
		GLenum ConvertUsage(MeshBuffer::Usage usage);
		GLenum ConvertFormat(MeshBuffer::Format format);
	};

}