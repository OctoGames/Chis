#include "OgreSystem.h"

OgreSystem* OgreSystem::instance_ = nullptr;

void OgreSystem::init(std::string appname)
{
#if _DEBUG
	resourcesCfg_ = "resources_d.cfg";
	pluginsCfg_ = "plugins_d.cfg";
#else
	resourcesCfg_ = "resources.cfg";
	pluginsCfg_ = "plugins.cfg";
#endif

	appName_ = appname;
	fileSystemLayer_ = new Ogre::FileSystemLayer(appName_);
	loadPlugins();
	readDataFromFile();
	setupWindow();
	sceneManager_ = root_->createSceneManager();
	setUpResources();
	loadResources();

	SDL_Init(SDL_INIT_VIDEO);
	unsigned long hWnd = 0;
	window_->getCustomAttribute("WINDOW", &hWnd);
	SDL_CreateWindowFrom((void*)hWnd);
	if (fullScreen_) window_->setFullscreen(true, winWidth_, winHeight_);
}

OgreSystem * OgreSystem::Instance()
{
	if (instance_ == nullptr) instance_ = new OgreSystem();
	return instance_;
}

void OgreSystem::setupWindow()
{
	//Only one available render system on our proyect
	root_->setRenderSystem(*(root_->getAvailableRenderers().begin()));
	root_->initialise(false);

	//Window
	window_ = root_->createRenderWindow(appName_, winWidth_, winHeight_, false);

	window_->setActive(true);
	window_->setAutoUpdated(true);
	window_->setDeactivateOnFocusChange(false);
}

//Reads the data from a .config file given to setUp the screen
void OgreSystem::readDataFromFile()
{
	std::string configFilePath = fileSystemLayer_->getConfigFilePath("plugins.cfg");
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
	resourcesPath_ = fileSystemLayer_->getConfigFilePath("resources.cfg");
	Ogre::ConfigFile cf;

	if (Ogre::FileSystemLayer::fileExists(resourcesPath_))
	{
		cf.load(resourcesPath_);
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
			std::string auxPath = resourcesPath_;
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


void OgreSystem::loadPlugins()
{
	Ogre::String pluginsPath;
	pluginsPath = fileSystemLayer_->getConfigFilePath("plugins.cfg");

	root_ = new Ogre::Root(pluginsPath);
}