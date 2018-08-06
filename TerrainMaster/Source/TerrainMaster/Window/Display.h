#pragma once

#include <map>
#include <string>
#include <functional>

#include <SDL.h>

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include "CursorType.h"

namespace TM {

	enum class DisplayState {
		Normal, Maximized
	};

	class InputManager;

	class Display {
	public:
		Display();
		~Display();

		void Create();
		void UpdateEvents();
		void Update();
		void Destroy();

		void SetAutoSize();
		void SetSize(int width, int height);
		int GetWidth() const;
		int GetHeight() const;

		void SetResizable(bool resizable);
		bool IsResizable() const;

		void Stop();
		bool IsRunning() const;

		int GetMonitorCount() const;
		void GetMonitorProperties(int monitor, int* w, int* h, int* refreshRate) const;

		float GetPixelRatio();

#ifdef _WIN32
		HWND GetWindowHandle();
#endif // _WIN32

		void SetWindowState(DisplayState state);
		DisplayState GetDisplayState() const;

		void SetInputManager(InputManager* inputManager);

		void SetTitleAdditional(const std::string& title);

		void SetMouseGrabbed(bool state);

		SDL_Window* GetWindow() const;

		void SetCursor(const CursorType& cursor);
	private:
		SDL_Window * _window = nullptr;
		SDL_GLContext _glContext;
		InputManager* _inputManager = nullptr;
		std::map<int, SDL_Cursor*> _loadedCursors;
		DisplayState _state;
		std::string _title;
		int _width = 0;
		int _height = 0;
		bool _created;
		bool _resizable;
		bool _isRunning;
	};

}