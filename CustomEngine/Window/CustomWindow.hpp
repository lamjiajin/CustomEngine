#pragma once
/******************************************************************************/
/*!
\Filename				GameWindow.h
\Main author			Lam Jiajin


\Author1 work percentage:	100%



details: display window
All content © 2015 DigiPen Institute of Technology Singapore. All Rights Reserved
*/
/******************************************************************************/

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Windows.h>
#include <WinUser.h>
#include <string>

class GameWindow
{
public:
	static LRESULT CALLBACK StaticGameWindowHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK GameWindowHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	GameWindow() : m_fullscreen(false) {}

	std::string GetLastErrorAsString();
	/************************************/
	// functions to control the window // 
	void MoveBy(int x, int y);
	float x;
	float y;
	float z;
	void SetWindowVisible(bool);
	void DecreaseAlpha();
	void IncreaseAlpha();
	unsigned char opacity;
	/************************************/
	// functions to control what the window can do // 

	void EnableDragNDrop();
	void DisableDragNDrop();

	/************************************/
	// data regarding the window itself // 
	const char windowclassname[23]{ "defaultwindowclassname" };
	void init(const char* name, int x = 0, int y = 0, int w = 1600, int h = 900); // optional flags? );
	void Update(float);
	void KillWindow();
	void switchscreen();
	bool activated = false;
	bool paused = false;
	HMENU   hMenu;
	HDC       hDC;
	HMODULE hInstance;
	HWND hwnd;
	MSG Msg;
	DWORD TCount;
	int m_width;
	int m_height;
	bool close_window = false;
	bool m_fullscreen;
	bool SCursor = true;
	WINDOWPLACEMENT wplm;
	int ground_level;
};
void GetDesktopResolution(int& horizontal, int& vertical);
GameWindow* GetGameWindow();
HWND GetHWND();

#endif