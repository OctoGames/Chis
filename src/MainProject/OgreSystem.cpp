#include "OgreSystem.h"

OgreSystem::OgreSystem(std::string appName) : appName_(appName), mRoot(0), fullScreen_(false), winWidth_(800), winHeight_(600)
{
#if _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	//mRoot = new Ogre::Root(mPluginsCfg);
	mFSLayer = new Ogre::FileSystemLayer(appName);

	//Load all needed plugins and initialize the root
	loadPluggins();

	readDataFromFile(); //reads all the data needed to create the window

	//Create the window
	setWindow();

	//SceneManager
	sManager = mRoot->createSceneManager();

	//SetUp and Load all resources from config file if there's no problem on load
	setUpResources();
	loadResources();

	//Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	unsigned long hWnd = 0;

	//Get Ogre window attributes
	mWindow->getCustomAttribute("WINDOW", &hWnd);

	//Create a SDL window based on thos attributes to detect input
	SDL_CreateWindowFrom((void*)hWnd);

	//After we create the SDL window, we set the Ogre window mode to fullscreen if required to solve blackscreen bug
	if (fullScreen_)
		mWindow->setFullscreen(true, winWidth_, winHeight_);
}

void OgreSystem::setWindow()
{
	//Only one available render system on our proyect
	mRoot->setRenderSystem(*(mRoot->getAvailableRenderers().begin()));
	mRoot->initialise(false);

	//Window
	mWindow = mRoot->createRenderWindow(appName_, winWidth_, winHeight_, false);

	mWindow->setActive(true);
	mWindow->setAutoUpdated(true);
	mWindow->setDeactivateOnFocusChange(false);
}

//Reads the data from a .config file given to setUp the screen
void OgreSystem::readDataFromFile()
{

	std::string configFilePath = mFSLayer->getConfigFilePath("plugins.cfg");
	configFilePath.erase(configFilePath.find_last_of("\\") + 1, configFilePath.size() - 1);

	//Read the information to FullScreen
	std::string line;
	std::string fullScreen;

	line = findConfig("FULLSCREEN", configFilePath);

	fullScreen = readString(line);

	if (fullScreen == "TRUE")
		fullScreen_ = true;

	//Read data config to screen width
	line = findConfig("WIDTH", configFilePath);
	std::string auxString = readString(line);
	winWidth_ = std::stoi(auxString);

	//Read data config to screen height
	line = findConfig("HEIGHT", configFilePath);
	auxString = readString(line);
	winHeight_ = std::stoi(auxString);
}

std::string OgreSystem::findConfig(std::string config, std::string configFilePath)
{
	std::ifstream configFile;

	configFile.open(configFilePath + "Configuration.config");

	std::string auxString;
	std::string line;
	bool found = false;

	while (!configFile.eof() && !found)
	{
		//read data to fullScreen Mode
		std::getline(configFile, line);

		auxString = line;

		if (auxString.erase(auxString.find_last_of("="), auxString.size() - 1) == config)
		{
			found = true;
		}

	}

	configFile.close();

	return line;
}

//Read the string until find = sign. Then takes the whole right part and returns it
std::string OgreSystem::readString(std::string s)
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

//this method parses the resources.cfg file how? not 100% sure but it works fine
void OgreSystem::setUpResources()
{
	resourcesPath = mFSLayer->getConfigFilePath("resources.cfg");
	Ogre::ConfigFile cf;

	if (Ogre::FileSystemLayer::fileExists(resourcesPath))
	{
		cf.load(resourcesPath);
	}

	Ogre::String sec, type, arch;
	// go through all specified resource groups
	Ogre::ConfigFile::SettingsBySection_::const_iterator seci;
	for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci)
	{
		sec = seci->first;
		const Ogre::ConfigFile::SettingsMultiMap& settings = seci->second;
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

		// go through all resource paths
		for (i = settings.begin(); i != settings.end(); i++)
		{
			type = i->first;

			//Added the whole route to fix the problem with other computers route
			std::string auxPath = resourcesPath;
			//We take the resources path and delete the finel path
			auxPath.erase(auxPath.find_last_of("\\") + 1, auxPath.size() - 1);

			//then, to taht last path, we add the one where resources will be located
			arch = auxPath + Ogre::FileSystemLayer::resolveBundlePath(i->second);
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
		}
	}

	sec = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	const Ogre::ResourceGroupManager::LocationList genLocs = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(sec);

	OgreAssert(!genLocs.empty(), ("Resource Group '" + sec + "' must contain at least one entry").c_str());

	arch = genLocs.front().archive->getName();
	type = genLocs.front().archive->getType();
}

void OgreSystem::loadResources()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

//Load all needed plugins and initialize the root
void OgreSystem::loadPluggins()
{
	Ogre::String pluginsPath;
	pluginsPath = mFSLayer->getConfigFilePath("plugins.cfg");

	mRoot = new Ogre::Root(pluginsPath);
}