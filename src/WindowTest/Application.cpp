#include "Application.h"


Application::Application(std::string appName) : appName_(appName), mRoot(0), fullScreen_(false), winWidth_(800), winHeight_(600)
{
#if _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
	mRoot = new Ogre::Root(mPluginsCfg);
	mRoot->loadPlugin("RenderSystem_GL_d");
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
	mRoot = new Ogre::Root(mPluginsCfg);
	mRoot->loadPlugin("RenderSystem_GL");
#endif

	readDataFromFile();

	mRoot->setRenderSystem(*(mRoot->getAvailableRenderers().begin()));
	mRoot->initialise(false);

	//Window
	mWindow = mRoot->createRenderWindow(appName_, winWidth_, winHeight_, false);

	mWindow->setActive(true);
	mWindow->setAutoUpdated(true);
	mWindow->setDeactivateOnFocusChange(false);

	if(fullScreen_)
		mWindow->setFullscreen(true, 1920, 1080);

	//SceneManager
	sManager = mRoot->createSceneManager();

	// viewport and camera
	Ogre::Camera *mainCamera = sManager->createCamera("mainCamera");
	Ogre::Viewport *viewport = mWindow->addViewport(mainCamera);
	viewport->setClearEveryFrame(true);
}


Application::~Application()
{
	delete mRoot;
}

//Reads the data from a .config file given to setUp the screen
void Application::readDataFromFile()
{
	std::ifstream configFile;
	configFile.open("../../bin/Configuration.config");
	
	std::string line;

	//read data to fullScreen Mode
	getline(configFile, line);
	std::string fullScreen = readString(line);

	if (fullScreen == "TRUE")
		fullScreen_ = true;
	
	//Read data config to screen width
	getline(configFile, line);
	std::string auxString = readString(line);
	winWidth_ = std::stoi(auxString);

	//Read data config to screen height
	getline(configFile, line);
	auxString = readString(line);
	winHeight_ = std::stoi(auxString);

	configFile.close();
}

//Read the string until find = sing. Then takes the whole right part and returns it
std::string Application::readString(std::string s)
{
	int i = 0;
	bool found = false;
	std::string auxString;

	while (i < s.size() && !found)
	{
		if (s[i] == '=')
		{
			found = true;
			auxString = s.substr(i + 1, s.size() - 1);
		}
		else i++;
	}

	return auxString;
}

