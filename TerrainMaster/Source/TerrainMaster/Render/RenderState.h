#pragma once

namespace TM {
	enum class BlendMode {
		Off,
		Additive,
		PremultAlpha,
		AlphaAdditive,
		Color,
		Alpha,
		Screen
	};

	enum class FaceCull {
		Off,
		Front,
		Back,
		FrontAndBack
	};

	struct RenderState {
		BlendMode BlendMode = BlendMode::Off;
		FaceCull FaceCull = FaceCull::Back;
		bool Wireframe = false;
		bool DepthTest = true;
		float LineWidth = 1.0f;
	};

	extern RenderState DefaultRenderState;
}