#include <iostream>
#include "Window/CustomWindow.hpp"
int main(void)
{
	std::cout << "hello world" << std::endl;
	GameWindow mainWindow;
	int h = 0, v = 0;
	GetDesktopResolution(h, v);
	std::cout << h << ", " << v << std::endl;
	
	mainWindow.init("FirstEngine");
	//mainWindow.EnableDragNDrop();
	while (true)
	{
		mainWindow.Update(0.f);
	}
	return 1;
}