#include "RenderManager.h"

RenderManager* RenderManager::instance_ = nullptr;

RenderManager* RenderManager::Instance()
{
	if (instance_ == nullptr) instance_ = new RenderManager();
	return instance_;
}

RenderManager::RenderManager() :
	time_(nullptr),
	window_(nullptr),
	running_(false),
	appName_(""),
	pluginsCfg_(""),
	resourcesCfg_(""),
	solutionPath_(""),
	root_(nullptr),
	sceneManager_(nullptr),
	fileSystemLayer_(nullptr)
{
}

void RenderManager::init()
{
	appName_ = "CHIS v0.5.0";

	initRoot();

	window_ = new Window(appName_);

	loadResources();

	time_ = new TimeManager(root_->getTimer());

	sceneManager_ = root_->createSceneManager();

	running_ = true;

	root_->clearEventTimes();
}

void RenderManager::update(float deltaTime)
{
	window_->update();
	root_->renderOneFrame();
}

void RenderManager::close()
{
	if (window_)
	{
		delete window_;
		window_ = nullptr;
	}

	if (root_)
	{
		root_->saveConfig();
		root_->destroySceneManager(sceneManager_);
		delete fileSystemLayer_;
		delete root_;
	}
}

void RenderManager::initRoot()
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

void RenderManager::loadResources()
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

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}