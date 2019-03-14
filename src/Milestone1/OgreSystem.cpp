#include "OgreSystem.h"

#include <SDL_syswm.h>
#include <OgreConfigFile.h>

OgreSystem* OgreSystem::instance_ = nullptr;

OgreSystem* OgreSystem::Instance()
{
	if (instance_ == nullptr) instance_ = new OgreSystem();
	return instance_;
}

OgreSystem::OgreSystem() :
	root_(nullptr),
	window_(nullptr),
	sceneManager_(nullptr),
	fileSystemLayer_(nullptr)
{

}

void OgreSystem::init()
{
	createRoot();
	createWindow();
	createResources();
	createSceneManager();
}

void OgreSystem::close()
{
	if (root_ != nullptr) root_->saveConfig();

	root_->destroySceneManager(sceneManager_);
	sceneManager_ = nullptr;

	delete window_;
	window_ = nullptr;

	delete fileSystemLayer_;
	fileSystemLayer_ = nullptr;

	delete root_;
	root_ = nullptr;
}

void OgreSystem::createRoot()
{
	fileSystemLayer_ = new Ogre::FileSystemLayer(appName_);

#if _DEBUG
	root_ = new Ogre::Root("plugins_d.cfg");
	solutionPath_ = fileSystemLayer_->getConfigFilePath("plugins_d.cfg");
#else
	root_ = new Ogre::Root("plugins.cfg");
	solutionPath_ = fileSystemLayer_->getConfigFilePath("plugins.cfg");
#endif

	fileSystemLayer_->setHomePath(solutionPath_);
	root_->initialise(false);
}

void OgreSystem::createWindow()
{
	window_ = new Window();

	int w, h, flags;
	Ogre::String token;
	Ogre::NameValuePairList miscParams;
	Ogre::ConfigOptionMap ropts = root_->getRenderSystem()->getConfigOptions();
	miscParams["FSAA"] = ropts["FSAA"].currentValue;
	miscParams["vsync"] = ropts["VSync"].currentValue;
	miscParams["gamma"] = ropts["sRGB Gamma Conversion"].currentValue;
	std::istringstream mode(ropts["Video Mode"].currentValue); mode >> w >> token >> h;
	if (ropts["Full Screen"].currentValue == "Yes") flags = SDL_WINDOW_FULLSCREEN;
	else flags = SDL_WINDOW_RESIZABLE;

	window_->createNativeWindow(appName_, w, h, flags);

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window_->getNativeWindow(), &wmInfo);
	miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));
	Ogre::RenderWindow* renderWindow = root_->createRenderWindow(appName_, w, h, false, &miscParams);
	
	window_->setRenderWindow(renderWindow);
	window_->setWindowGrab(false, true);
}

void OgreSystem::createResources()
{
	Ogre::ConfigFile cf;
	Ogre::String resourcesPath = fileSystemLayer_->getConfigFilePath("resources.cfg");
	if (Ogre::FileSystemLayer::fileExists(resourcesPath)) cf.load(resourcesPath);
	else Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
			Ogre::FileSystemLayer::resolveBundlePath(solutionPath_ + "\\Assets"),
			"FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	Ogre::String sec, type, arch;
	Ogre::ConfigFile::SettingsBySection_::const_iterator seci;
	for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci) 
	{
		sec = seci->first;
		const Ogre::ConfigFile::SettingsMultiMap& settings = seci->second;
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

		for (i = settings.begin(); i != settings.end(); i++)
		{
			type = i->first;
			arch = Ogre::FileSystemLayer::resolveBundlePath(i->second);
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
		}
	}

	sec = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	const Ogre::ResourceGroupManager::LocationList genLocs = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(sec);
	OgreAssert(!genLocs.empty(), ("Resource Group '" + sec + "' must contain at least one entry").c_str());
	arch = genLocs.front().archive->getName();
	type = genLocs.front().archive->getType();

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void OgreSystem::createSceneManager()
{
	sceneManager_ = root_->createSceneManager();
}