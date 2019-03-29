#include "Application.h"

int main(int argc, char *argv[])
{
	try 
	{
		Application app;
		app.run();
	}
	catch (Ogre::Exception& e)
	{
		Ogre::LogManager::getSingleton().logMessage("Ogre exception: " + e.getFullDescription() + "\n");
	}

	return 0;
}