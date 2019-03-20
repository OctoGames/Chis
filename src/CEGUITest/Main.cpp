#include "Application.h"

int main(int argc, char *argv[])
{
	Application* app = nullptr;

	try
	{
		app = new Application();
		app->run();
	}
	catch (Ogre::Exception& e)
	{
		Ogre::LogManager::getSingleton().logMessage("An exception has occured: " + e.getFullDescription() + "\n");
	}
	catch (std::exception& e)
	{
		std::string w = e.what();
		Ogre::LogManager::getSingleton().logMessage("An exception has occured: " + w + "\n");
	}

	delete app;

	return 0;
}