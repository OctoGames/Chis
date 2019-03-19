#include "Application.h"
#include "OgreSystem.h"
//Garbage collector
//#include "CheckML.h"

int main(int argc, char* argv[])
{
	//Garbage collector
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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