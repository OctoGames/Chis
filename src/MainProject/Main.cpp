#include "Application.h"
#include "RenderManager.h"

int main(int argc, char* argv[])
{
	try
	{
		Application app;
		app.run();
	}
	catch (Ogre::Exception& e)
	{
		Ogre::LogManager::getSingleton().logMessage("An exception has occured: " + e.getFullDescription() + "\n");
	}

	return 0;
}