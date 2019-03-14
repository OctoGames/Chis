#include "Application.h"
#include "OgreSystem.h"

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

	delete app;

	return 0;
}