#include "Display.h"

#include <iostream>
#include <GL/glew.h>

#ifdef _WIN32
#include <Windows.h>
//#include "../resource.h"

#include "SDL_syswm.h"
#include "wtypes.h"
#endif // _WIN32

namespace TM {
	Display::Display() : _state(DisplayState::Normal), _title("NovaEngine 2.0") {
		_window = nullptr;
		_inputManager = nullptr;
		_width = 1280;
		_height = 720;
		_created = false;
		_resizable = false;
		_isRunning = false;
		SDL_Init(SDL_INIT_EVERYTHING);
		SetAutoSize();
	}

	Display::~Display() {
		for (std::map<int, SDL_Cursor*>::iterator i = _loadedCursors.begin(); i != _loadedCursors.end(); i++) {
			SDL_FreeCursor(i->second);
		}
		Destroy();
	}

	void Display::Create() {
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
		SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);

		bool m_bDebugOpenGL = false;

		if (m_bDebugOpenGL)
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

		int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

		if (_resizable) {
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if (_state == DisplayState::Maximized) {
			flags |= SDL_WINDOW_MAXIMIZED;
		}

		if (_width == 0 || _height == 0) {
			std::cerr << "Display size cannot be 0 !" << std::endl;
			return;
		}

		_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, flags);
		if (_state == DisplayState::Maximized) {
			GetMonitorProperties(0, &_width, &_height, NULL);
		}

#ifdef _WIN32
		/*HWND hWnd = getWindowHandle();
		HMENU hMenuBar = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU2));
		SetMenu(hWnd, hMenuBar);*/
#endif // _WIN32

		//SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

		/*SDL_SetWindowsMessageHook([](void* userdata, void* hWnd, unsigned int message, Uint64 wParam, Sint64 lParam) {
		Display* display = (Display*)userdata;
		UINT msg = (UINT)message;
		switch (msg) {
		case WM_COMMAND:
		WORD w = LOWORD(wParam);
		if (display->_menuListener) {
		display->_menuListener(w);
		}
		break;
		}
		}, this);*/

		_glContext = SDL_GL_CreateContext(_window);
		glewExperimental = true;
		GLenum status = glewInit();
		if (status != GLEW_OK) {
			std::cerr << "Glew failed to initialize !" << std::endl;
		}

		//glDisable(GL_FRAMEBUFFER_SRGB);
		//glEnable(GL_FRAMEBUFFER_SRGB);
		SDL_GL_SetSwapInterval(1);

		_isRunning = true;
		Update();
	}

	void Display::SetMouseGrabbed(bool state) {
		SDL_SetRelativeMouseMode(state ? SDL_TRUE : SDL_FALSE);
	}

	void Display::UpdateEvents() {
		SDL_PumpEvents();
		int mouseX = 0;
		int mouseY = 0;
		SDL_GetMouseState(&mouseX, &mouseY);
		if (_inputManager != nullptr) {
			//_inputManager->setMouseLocation(mouseX, mouseY);
		}

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				_isRunning = false;
			}
			
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				_width = e.window.data1;
				_height = e.window.data2;
				/*if (_resizeListener) {
					_resizeListener(_width, _height);
				}*/
			}
			if (e.type == SDL_DROPFILE) {
				/*if (_fileDropListener) {
					_fileDropListener(std::string(e.drop.file), mouseX, mouseY);
				}*/
			}
			/*if (_inputManager != nullptr)
				_inputManager->processEvent(e);*/
		}
	}

	void Display::Update() {
		SDL_GL_SwapWindow(_window);
	}

	void Display::Destroy() {
		SDL_GL_DeleteContext(_glContext);
		SDL_DestroyWindow(_window);
	}

	void Display::SetAutoSize() {
		int mw = 0;
		int mh = 0;
		GetMonitorProperties(0, &mw, &mh, NULL);
		float dx = (float)mw / 100.0f;
		float dy = (float)mh / 100.0f;
		float scaleMode = 75.0f;
		_width = (int)(dx * scaleMode);
		_height = (int)(dy * scaleMode);
	}

	void Display::SetSize(int width, int height) {
		_width = width;
		_height = height;
	}

	int Display::GetWidth() const {
		return _width;
	}

	int Display::GetHeight() const {
		return _height;
	}

	void Display::SetResizable(bool resizable) {
		_resizable = resizable;
	}

	bool Display::IsResizable() const {
		return _resizable;
	}

	void Display::Stop() {
		_isRunning = false;
	}

	bool Display::IsRunning() const {
		return _isRunning;
	}

	int Display::GetMonitorCount() const {
		return SDL_GetNumVideoDisplays();
	}

	void Display::GetMonitorProperties(int monitor, int * w, int * h, int * refreshRate) const {
		SDL_DisplayMode current;
		int should_be_zero = SDL_GetCurrentDisplayMode(monitor, &current);
		if (should_be_zero == NULL) {
			if (w != nullptr) {
				*w = current.w;
			}
			if (h != nullptr) {
				*h = current.h;
			}
			if (refreshRate != nullptr) {
				*refreshRate = current.refresh_rate;
			}
		}
	}

	float Display::GetPixelRatio() {
		int mFBSize;
		int mSize;
		SDL_GL_GetDrawableSize(_window, &mFBSize, NULL);
		SDL_GetWindowSize(_window, &mSize, NULL);

		return (float)mFBSize / (float)mSize;
	}

#ifdef _WIN32
	HWND Display::GetWindowHandle() {
		struct SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		if (-1 == SDL_GetWindowWMInfo(_window, &wmInfo)) {
			return NULL;
		}
		return wmInfo.info.win.window;
	}
#endif // _WIN32

	void Display::SetWindowState(DisplayState state) {
		_state = state;
	}

	DisplayState Display::GetDisplayState() const {
		return _state;
	}

	void Display::SetInputManager(InputManager* inputManager) {
		_inputManager = inputManager;
	}

	void Display::SetTitleAdditional(const std::string& title) {
		SDL_SetWindowTitle(_window, ("NovaEngine 2.0" + title).c_str());
	}

	SDL_Window* Display::GetWindow() const {
		return _window;
	}
	void Display::SetCursor(const CursorType& cursor) {
		SDL_Cursor* c = nullptr;
		if (_loadedCursors.find((int)cursor) != _loadedCursors.end()) {
			c = _loadedCursors[(int)cursor];
		}
		else {
			c = SDL_CreateSystemCursor((SDL_SystemCursor)cursor);
			_loadedCursors[(int)cursor] = c;
		}
		if (c != nullptr) {
			SDL_SetCursor(c);
		}
	}
}