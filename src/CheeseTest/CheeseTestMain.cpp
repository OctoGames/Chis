#include "CheeseTestApp.h"

int main(int argc, char *argv[])
{
	std::string appName = "CHIS! (Test App v0.5)";
	CheeseTestApp chisApp(appName);

	try 
	{
		chisApp.init();
		chisApp.getRoot()->startRendering();
	}
	catch (Ogre::Exception& e) 
	{
		Ogre::LogManager::getSingleton().logMessage(
			"An exception has occured: " + e.getFullDescription() + "\n");
	}

	chisApp.close();

	return 0;
}