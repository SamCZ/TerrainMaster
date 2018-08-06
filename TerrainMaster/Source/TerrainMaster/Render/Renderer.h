#pragma once

#include "TerrainMaster/Utils/ColorRGBA.h"
#include "RenderState.h"

namespace TM {

	class Renderer {
	private:

	public:
		Renderer();
		~Renderer();

		void ClearScreen(bool color, bool depth, bool stencil);
		void ClearColor(float r, float g, float b, float a);
		void ClearColor(ColorRGBA color);


	};

}