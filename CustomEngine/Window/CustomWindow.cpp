#include <Windows.h>
#include <Shellapi.h>
#include <string>
#include <vector>
#include <iostream>
#include "CustomWindow.h"
#include "../Input/Input.h"
#define IDR_MENU 10000
#define IDM_FILE 10001
#define IDM_QUIT 10002

/********************************* Globals **********************************/

GameWindow* GameWindow::GameWindowInstance = nullptr;


void GameWindow::init(const char* name, int X, int Y, int w, int h)
{
	m_width = w;
	m_height = h;
	x = (float)X;
	y = (float)Y;
	hInstance = GetModuleHandle(NULL);
	// get our instance handle
	WNDCLASSEX  wc;    // window class
	int width_of_screen = 0;
	GetDesktopResolution(width_of_screen, ground_level);
	// fill out the window class structure
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = StaticGameWindowHandler;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);  // default icon
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);      // windows logo small icon // unique to wndclassex only
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);      // default arrow
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);;     // no background needed
	wc.lpszMenuName = NULL;     // no menu
	wc.lpszClassName = windowclassname;
	
	// register the windows class
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Unable to register the window class", "Error", MB_OK | MB_ICONEXCLAMATION);
		// exit and return false
		std::exit(1);
	}

	// if we're in fullscreen mode, set the display up for it
	if (m_fullscreen)
	{
		// set up the device mode structure
		DEVMODE screenSettings;
		memset(&screenSettings, 0, sizeof(screenSettings));
		m_width = width_of_screen;
		m_height = ground_level;
		screenSettings.dmSize = sizeof(screenSettings);
		screenSettings.dmPelsWidth = m_width;    // screen width
		screenSettings.dmPelsHeight = m_height;   // screen height
		screenSettings.dmBitsPerPel = 32;     // bits per pixel
		screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// attempt to switch to the resolution and bit depth we've selected
		if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// if we can't get fullscreen, let them choose to quit or try windowed mode
			if (MessageBox(NULL, "Cannot run in the fullscreen mode at the selected resolution\n"
				"on your video card. Try windowed mode instead?",
				"Warning",
				MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
				m_fullscreen = false;
			else
				std::exit(1);
		}
	}

	DWORD dwExStyle;
	DWORD dwStyle;

	// set the window style appropriately, depending on whether we're in fullscreen mode
	if (m_fullscreen)
	{
		SCursor = false;
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;        // simple window with no borders or title bar
		ShowCursor(SCursor);          // hide the cursor for now
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_LAYERED;
		dwStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME  & ~WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	}

	// set up the window we're rendering to so that the top left corner is at (0,0)
	// and the bottom right corner is (height,width)
	RECT  windowRect;
	windowRect.left = (LONG)x;
	windowRect.right = (LONG)m_width + (LONG)x;
	windowRect.top = (LONG)y;
	windowRect.bottom = (LONG)m_height + (LONG)y;

	// change the size of the rect to account for borders, etc. set by the style
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	//hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));

	// class registered, so now create our window
	hwnd = CreateWindowEx(dwExStyle,          // extended style
		windowclassname,     // class name
		name,                // app name
		dwStyle,
		0, 0,               // x,y coordinate
		windowRect.right - windowRect.left, // width
		windowRect.bottom - windowRect.top, // height
		GetDesktopWindow(),               // handle to parent
		NULL,              // handle to menu, use NULL if not needed
		hInstance,          // application instance
		this);              // no extra params


	opacity = 255;
	SetLayeredWindowAttributes(hwnd, RGB(1, 55, 60), opacity, LWA_ALPHA | LWA_COLORKEY);

	// see if our window handle is valid
	if (!hwnd)
	{
		MessageBox(NULL, "Unable to create window", "Error", MB_OK | MB_ICONEXCLAMATION);
	}

	// get a device context
	if (!(hDC = GetDC(hwnd)))
	{
		MessageBox(NULL, "Unable to create device context", "Error", MB_OK | MB_ICONEXCLAMATION);
	}


	// set the pixel format we want
	//PIXELFORMATDESCRIPTOR pfd = {
	//	sizeof(PIXELFORMATDESCRIPTOR),  // size of structure
	//	1,                              // default version
	//	PFD_DRAW_TO_WINDOW |            // window drawing support
	//	PFD_SUPPORT_OPENGL |            // OpenGL support
	//	PFD_DOUBLEBUFFER,               // double buffering support
	//	PFD_TYPE_RGBA,                  // RGBA color mode
	//	32,                           // 32 bit color mode
	//	0, 0, 0, 0, 0, 0,               // ignore color bits, non-palettized mode
	//	0,                              // no alpha buffer
	//	0,                              // ignore shift bit
	//	0,                              // no accumulation buffer
	//	0, 0, 0, 0,                     // ignore accumulation bits
	//	16,                             // 16 bit z-buffer size
	//	8,                              // no stencil buffer
	//	0,                              // no auxiliary buffer
	//	PFD_MAIN_PLANE,                 // main drawing plane
	//	0,                              // reserved
	//	0, 0, 0 };                      // layer masks ignored

	//GLuint  pixelFormat;

	//// choose best matching pixel format
	//if (!(pixelFormat = ChoosePixelFormat(hDC, &pfd)))
	//{
	//	MessageBox(NULL, "Can't find an appropriate pixel format", "Error", MB_OK | MB_ICONEXCLAMATION);
	//}

	//// set pixel format to device context
	//if (!SetPixelFormat(hDC, pixelFormat, &pfd))
	//{
	//	MessageBox(NULL, "Unable to set pixel format", "Error", MB_OK | MB_ICONEXCLAMATION);
	//}


	//// create the OpenGL rendering context
	//if (!(hRC = wglCreateContext(hDC)))
	//{
	//	MessageBox(NULL, "Unable to create OpenGL rendering context", "Error", MB_OK | MB_ICONEXCLAMATION);
	//}

	//// now make the rendering context the active one
	//if (!wglMakeCurrent(hDC, hRC))
	//{
	//	MessageBox(NULL, "Unable to activate OpenGL rendering context", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	//}

	// show the window in the forground, and set the keyboard focus to it
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	GameWindowInstance = this;
}

GameWindow* GetGameWindow()
{
	//return &gameWindow;
	return GameWindow::GameWindowInstance;
}

HWND GetHWND() { return GetGameWindow()->hwnd; }

std::string GameWindow::GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}


void GameWindow::switchscreen()
{
	//DWORD dwExStyle;
	//DWORD dwStyle;

	//if (m_fullscreen)
	//{
	//	ShowCursor(SCursor);

	//	ChangeDisplaySettings(NULL, CDS_RESET);

	//	//dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	//	//dwStyle = WS_OVERLAPPEDWINDOW;
	//	dwExStyle = WS_OVERLAPPEDWINDOW | WS_EX_WINDOWEDGE | WS_EX_LAYERED;
	//	dwStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME  & ~WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	//	SetWindowLongPtr(hwnd, GWL_STYLE, dwStyle);
	//	//SetWindowLongPtr(hwnd, GWL_EXSTYLE, dwExStyle);

	//	SetWindowPlacement(hwnd, &wplm);
	//	RECT  windowRect;
	//	windowRect.left = (LONG)x;
	//	windowRect.right = (LONG)m_width + (LONG)x;
	//	windowRect.top = (LONG)y;
	//	windowRect.bottom = (LONG)m_height + (LONG)y;

	//	// change the size of the rect to account for borders, etc. set by the style
	//	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	//	SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, SWP_SHOWWINDOW);

	//	ShowWindow(hwnd, SW_RESTORE);

	//}
	//else
	//{
	//	ShowCursor(SCursor);

	//	// set up the device mode structure
	//	DEVMODE screenSettings;
	//	memset(&screenSettings, 0, sizeof(screenSettings));
	//	int width;
	//	int height;
	//	GetDesktopResolution(width, height);
	//	screenSettings.dmSize = sizeof(screenSettings);
	//	screenSettings.dmPelsWidth = width;    // screen width
	//	screenSettings.dmPelsHeight = height;   // screen height
	//	screenSettings.dmBitsPerPel = 32;     // bits per pixel
	//	screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	//	// attempt to switch to the resolution and bit depth we've selected
	//	if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	//	{
	//		// if we can't get fullscreen, let them choose to quit or try windowed mode
	//		if (MessageBox(NULL, L"Cannot run in the fullscreen mode at the selected resolution\n"
	//			"on your video card. Try windowed mode instead?",
	//			L"Warning",
	//			MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	//			m_fullscreen = false;
	//		else
	//			return;
	//	}

	//	dwExStyle = WS_EX_APPWINDOW;
	//	dwStyle = WS_POPUP;           // simple window with no borders or title bar

	//	MONITORINFOEX mInfo;
	//	mInfo.cbSize = sizeof(MONITORINFOEX);
	//	wplm.length = sizeof(WINDOWPLACEMENT);
	//	GetWindowPlacement(hwnd, &wplm);
	//	GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &mInfo);
	//	SetWindowLongPtr(hwnd, GWL_STYLE, dwStyle);
	//	SetWindowLongPtr(hwnd, GWL_EXSTYLE, dwExStyle);

	//	SetWindowPos(
	//		hwnd,
	//		HWND_TOP,
	//		mInfo.rcMonitor.left,
	//		mInfo.rcMonitor.top,
	//		mInfo.rcMonitor.right - mInfo.rcMonitor.left,
	//		mInfo.rcMonitor.bottom - mInfo.rcMonitor.top,
	//		SWP_NOOWNERZORDER | SWP_FRAMECHANGED
	//	);

	//	ShowWindow(hwnd, SW_MAXIMIZE);
	//}

	//m_fullscreen = !m_fullscreen;
}


void GameWindow::MoveBy(int X, int Y)
{
	x += X;
	y -= Y;
	MoveWindow(hwnd, (int)x, (int)y, m_width, m_height, true);
}
void GameWindow::SetWindowVisible(bool a)
{
	unsigned char r = a ? 255 : 0;
	opacity = r;
	SetLayeredWindowAttributes(hwnd, RGB(1, 60, 90), r, LWA_ALPHA | LWA_COLORKEY);
}
void GameWindow::IncreaseAlpha()
{
	if (opacity < 255)
		SetLayeredWindowAttributes(hwnd, RGB(1, 60, 90), ++opacity, LWA_ALPHA | LWA_COLORKEY);
}
void GameWindow::DecreaseAlpha()
{
	if (opacity > 0)
		SetLayeredWindowAttributes(hwnd, RGB(1, 60, 90), --opacity, LWA_ALPHA | LWA_COLORKEY);
}

void GameWindow::EnableDragNDrop()
{
	DragAcceptFiles(hwnd, true);
}
void GameWindow::DisableDragNDrop()
{
	DragAcceptFiles(hwnd, false);
}


void window_move_WSAD_P_to_pause()
{
	static bool pause = true;
	if (input_mgr::IsTriggered(Key::P))
	{
		pause = !pause;
	}
	if (!pause && (GetGameWindow()->y < GetGameWindow()->ground_level - GetGameWindow()->m_height))
	{
		GetGameWindow()->MoveBy(0, -5);
	}
	if (input_mgr::IsHeld(Key::W))
	{
		GetGameWindow()->MoveBy(0, 10);
	}
	if (input_mgr::IsHeld(Key::S))
	{
		GetGameWindow()->MoveBy(0, -10);
	}
	if (input_mgr::IsHeld(Key::A))
	{
		GetGameWindow()->MoveBy(-10, 0);
	}
	if (input_mgr::IsHeld(Key::D))
	{
		GetGameWindow()->MoveBy(10, 0);
	}

}
void numpadeight_nine_setalpha()
{
	if (input_mgr::IsHeld(VK_NUMPAD8))
	{
		GetGameWindow()->DecreaseAlpha();
	}
	if (input_mgr::IsHeld(VK_NUMPAD9))
	{
		GetGameWindow()->IncreaseAlpha();
	}

}



void GameWindow::Update(float)
{
	window_move_WSAD_P_to_pause();
	numpadeight_nine_setalpha();
	while (PeekMessage(&Msg, NULL, NULL, NULL, PM_REMOVE))
	{
		if (Msg.message == WM_QUIT)   // do we receive a WM_QUIT message?
		{
			std::exit(1);              // if so, time to quit the application
		}
		else
		{
			TranslateMessage(&Msg);     // translate and dispatch to event queue
			DispatchMessage(&Msg);
		}
	}
}


void GameWindow::KillWindow()
{
	DestroyMenu(hMenu);
	//	ImGui_ImplDX11_Shutdown();
	// restore the original display if we're in fullscreen mode
	if (m_fullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(SCursor);
	}
	//
	// if we have an RC, release it
	//if (hRC)
	//{
	//	// release the RC
	//	if (!wglMakeCurrent(NULL, NULL))
	//		MessageBox(NULL, "Unable to release rendering context", "Error", MB_OK | MB_ICONINFORMATION);

	//	// delete the RC
	//	if (!wglDeleteContext(hRC))
	//		MessageBox(NULL, "Unable to delete rendering context", "Error", MB_OK | MB_ICONINFORMATION);

	//	hRC = NULL;
	//}

	// release the DC if we have one
	if (hDC && !ReleaseDC(hwnd, hDC))
	{
		MessageBox(NULL, "Unable to release device context", "Error", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;
	}

	// destroy the window if we have a valid handle
	if (hwnd && !DestroyWindow(hwnd))
	{
		MessageBox(NULL,"Unable to destroy window", "Error", MB_OK | MB_ICONINFORMATION);
		hwnd = NULL;
	}

	// unregister our class so we can create a new one if we need to
	if (!UnregisterClass(windowclassname, hInstance))
	{
		MessageBox(NULL, "Unable to unregister window class", "Error", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;
	}
}


void StorePathToVec(const std::string str, std::vector<std::string>& vec)
{
	//size_t numSlash = std::count(str.begin(), str.end(), '\\');

	//size_t end, start;

	////numSlash + 1 because of 2words per slash
	//for (size_t i = 0; i < numSlash + 1; ++i)
	//{
	//	if (i == 0)
	//	{
	//		end = str.length();
	//	}

	//	start = str.find_last_of('\\', end);

	//	if (str[end] == '\\')
	//		end--;

	//	vec.push_back(str.substr(start + 1, end - start));
	//	end = start - 1;
	//}
}


LRESULT CALLBACK GameWindow::StaticGameWindowHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	GameWindow* pParent;
	//ImGui_ImplDX11_WndProcHandler(hWnd, uMsg, wParam, lParam);
	// Get pointer to window
	if (uMsg == WM_CREATE)
	{
		pParent = (GameWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pParent);
	}
	else
	{
		pParent = (GameWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (!pParent) return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	pParent->hwnd = hWnd;
	return pParent->GameWindowHandler(hWnd, uMsg, wParam, lParam);
}





LRESULT CALLBACK GameWindow::GameWindowHandler(
	HWND hWnd,		//The window the message is for (ours in this case)
	UINT msg,		//The message type
	WPARAM wParam,	//The primary data for the message
	LPARAM lParam)	//The secondary data for the message (if any)
{
	//GetIMGUIWndProc(hWnd, msg, wParam, lParam);

	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_ACTIVATEAPP:
	case WM_ACTIVATE:
	{
		switch (wParam)
		{
		case 0:  //FALSE or WM_INACTIVE
				 //Pause game, etc
			if (activated)
			{
				//fullScreen = GET_CORE.GetGraphicsManager()->IsFullScreen();
				activated = false;/*
				input_mgr::active = false;
				EnginePause = true;
				fullScreen = GET_CORE.GetGraphicsManager()->IsFullScreen();*/
				if (m_fullscreen)
				{

					//GET_CORE.GetGraphicsManager()->ToggleFullScreen(false);
					ShowWindow(hWnd, SW_MINIMIZE);
				}
/*
				auto am = GET_CORE.GetAudioManager();
				if (am)am->PauseAll();*/

			}
			break;

		case 1:  //TRUE or WM_ACTIVE or WM_CLICKACTIVE
		case 2:
			//Reactivate, reset lost device, etc

			if (!activated)
			{
				//EnginePause = false;
				//input_mgr::active = true;

				ShowWindow(hWnd, SW_SHOWNORMAL);
#if _SHIPPING
				if (m_fullscreen)
				{
					GET_CORE.GetGraphicsManager()->ToggleFullScreen(true);
				}
#endif
				//auto am = GET_CORE.GetAudioManager();
				//if (am)am->UnPauseAll();
				activated = true;
			}
			break;
		default: break;
		}
	} break;

	case WM_DROPFILES:
	{
		HDROP hDrop = reinterpret_cast<HDROP>(wParam);
		UINT count = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
		for (UINT i = 0; i < count; ++i)
		{
			CHAR name[256];
			int size = 32767;

			if (DragQueryFile(hDrop, i, name, size))
			{
				std::string sourceFilePath(name);
				auto pos = sourceFilePath.find_last_of('\\') + 1;
				auto size = sourceFilePath.find_last_of('.') - pos;
				sourceFilePath = sourceFilePath.substr(pos, size);
				std::cout << sourceFilePath << std::endl;
				//CreatePrefab(sourceFilePath.c_str());
			}
		}

		DragFinish(hDrop);
		break;
	}


	case WM_MOUSEWHEEL:
	{
		Keyboard::wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam) / 120;
		break;
	}

	case WM_SYSCOMMAND:  // look for screensavers and powersave mode
	{
		switch (wParam)
		{
		case SC_SCREENSAVE:     // screensaver trying to start
		case SC_MONITORPOWER:   // monitor going to powersave mode
								// returning 0 prevents either from happening
			return 0;
		default:
			break;
		}
	} break;


	case WM_COMMAND:	// Handle messages from the Menu bar
	{
		// Get a handle to the menu
		HMENU hMenu = ::GetMenu(hwnd);

		switch (LOWORD(wParam))
		{
		case IDM_QUIT: // Activate the line DDA menu item
		{
			// send WM_QUIT to message queue
			PostQuitMessage(0);
			return 0;
		}
		}
		return 0;
	}

	case WM_CLOSE:    // window is being closed
	{
		if (MessageBox(hWnd, "Are you sure?","Quit Application", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
		{
			// inside will call destroywindow(), free the handles etc
			PostQuitMessage(0);
			KillWindow();
			//DestroyWindow(hWnd);
			return 0;
		}
		else
		{
			// do nothing. returns 0 to acknowledge msg is processed.
			return 0;
		}
		// send WM_QUIT to message queue
		//PostQuitMessage(0);
		//close_window = true;
		//GET_CORE.SetMsg(QUIT);
		//return 0;
	}

	case WM_SIZING:
	{
		RECT  temp = *((RECT*)lParam);
		m_width = temp.right - temp.left;
		m_height = temp.bottom - temp.top;
		return 0;
	}

	case WM_SIZE:
	{
		break;
		if (wParam == 0)
		{
			auto width = LOWORD(lParam);
			auto height = HIWORD(lParam);
			//std::cout << " wm size! " << "wparam :" << wParam << " width: " << width << " height: " << height << "\n";
			//if(GETSYSTEM(GraphicsManager))
			//GETSYSTEM(GraphicsManager)->HandleResize(width, height);
		}

	}

	case WM_KEYDOWN:
		input_mgr::keyboardMouse.currentKeys[wParam] = 1;


	//	break;

	case WM_KEYUP:
		input_mgr::keyboardMouse.currentKeys[wParam]  = 0;

	//	break;

	case WM_SYSKEYDOWN:
	{
		//Eat the WM_SYSKEYDOWN message to prevent freezing the game when
		//the alt key is pressed
		switch (wParam)
		{
		case VK_LMENU:
		case VK_RMENU:

			return 0;//Stop Alt key from stopping the winProc
		case VK_F4:
			//Check for Alt F4
			DWORD dwAltKeyMask = (1 << 29);
			//if ((lParam & dwAltKeyMask) != 0)
			//	GET_CORE.SetMsg(QUIT);
			return 0;
		}
		return 0;//
	}

	default:
		break;
	}




	return DefWindowProc(hWnd, msg, wParam, lParam);
}