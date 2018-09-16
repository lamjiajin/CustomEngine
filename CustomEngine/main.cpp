#include <iostream>
#include "Graphics/Graphics.h"
#include "Window/CustomWindow.h"
#include "Input/Input.h"
int main(void)
{
	std::cout << "hello world" << std::endl;
	GameWindow mainWindow;
	int h = 0, v = 0;
	GetDesktopResolution(h, v);
	std::cout << h << ", " << v << std::endl;
	
	mainWindow.init("FirstEngine",0,0,400,400);
	input_mgr::Init();
	//mainWindow.EnableDragNDrop();
	input_mgr::active = true;
	Graphics::init();
	while (true)
	{
		mainWindow.Update(0.f);
		input_mgr::Update();
	}
	return 1;
}