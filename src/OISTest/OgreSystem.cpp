#include "OgreSystem.h"

#include <SDL_syswm.h>
#include <OgreConfigFile.h>
#include <OgreGpuProgramManager.h>

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
	appName_ = "CHIS";
}

void OgreSystem::init()
{
	createRoot();
	createWindow();
	createInputManager();
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
	if (root_->restoreConfig()) root_->initialise(false);
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

void OgreSystem::createInputManager()
{
	size_t hWnd = 0;
	getWindow()->getCustomAttribute("WINDOW", &hWnd);

	inputManager_ = OIS::InputManager::createInputSystem(hWnd);
	mouse_ = static_cast<OIS::Mouse*>(inputManager_->createInputObject(OIS::OISMouse, false));
	keyboard_ = static_cast<OIS::Keyboard*>(inputManager_->createInputObject(OIS::OISKeyboard, false));
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


	//---------------------------------------------------------------SHADERS-------------------------------------------//
	// Add locations for supported shader languages

	if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSLES", type, sec);
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL120", type, sec);

		if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl150"))
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL150", type, sec);
		}
		else
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL", type, sec);
		}

		if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl400"))
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL400", type, sec);
		}
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/HLSL", type, sec);
	}

	std::string mRTShaderLibPath = arch + "/RTShaderLib";
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/materials", type, sec);

	if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSL", type, sec);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSLES", type, sec);
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSL", type, sec);
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/HLSL", type, sec);
	}


	//Initialize all resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void OgreSystem::createSceneManager()
{
	sceneManager_ = root_->createSceneManager();
}