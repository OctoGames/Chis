#include "ChisApp.h"

int main(int argc, char* argv[])
{
#ifndef _DEBUG
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, 0);
#else
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, 1);
#endif	// !_DEBUG

	try
	{
		ChisApp* app = new ChisApp();
		app->init();
		app->run();
		app->close();
		delete app;
	}
	catch (Ogre::Exception& e)
	{
		Ogre::LogManager::getSingleton().logMessage("An exception has occured: " + e.getFullDescription() + "\n");
	}

	return 0;
}