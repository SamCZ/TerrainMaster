#pragma once

#include <SDL.h>

namespace TM {
	enum class CursorType : int {
		Arrow = SDL_SYSTEM_CURSOR_ARROW,
		IBeam = SDL_SYSTEM_CURSOR_IBEAM,
		Wait = SDL_SYSTEM_CURSOR_WAIT,
		Cross = SDL_SYSTEM_CURSOR_CROSSHAIR,
		SmallWait = SDL_SYSTEM_CURSOR_WAITARROW,
		ScaleX = SDL_SYSTEM_CURSOR_SIZEWE,
		ScaleY = SDL_SYSTEM_CURSOR_SIZENS,
		ScaleAll = SDL_SYSTEM_CURSOR_SIZEALL,
		No = SDL_SYSTEM_CURSOR_NO,
		Hand = SDL_SYSTEM_CURSOR_HAND
	};
}