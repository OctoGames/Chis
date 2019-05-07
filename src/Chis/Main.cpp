#include "ChisApp.h"

int main(int argc, char* argv[])
{
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