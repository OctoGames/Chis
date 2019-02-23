#include "Application.h"


// OGRE Includes
#include <OgreConfigFile.h>
#include <OgreDataStream.h>
#include <OgreBitesConfigDialog.h>
#include <OgreGpuProgramManager.h>
#include <OgreWindowEventUtilities.h>

// SDL Includes
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_syswm.h>
#include <SDLInputMapping.h>

Application::Application(const Ogre::String appName) :
	mAppName(appName),
	mFirstRun(true),
	mRoot(nullptr),
	mOverlaySystem(nullptr),
	mFileSystemLayer(nullptr),
	mShaderGenerator(nullptr),
	mMaterialMgrListener(nullptr)
{
}

Application::~Application()
{
}

// INIT APP-------------------------------------------------------------------
void Application::init()
{
	createRoot();
	setup();
}

void Application::createRoot()
{
	mFileSystemLayer = new Ogre::FileSystemLayer(mAppName);

#if _DEBUG
	mRoot = new Ogre::Root("plugins_d.cfg");
	mSolutionPath = mFileSystemLayer->getConfigFilePath("plugins_d.cfg");
#else
	mRoot = new Ogre::Root("plugins.cfg");
	mSolutionPath = mFileSystemLayer->getConfigFilePath("plugins.cfg");
#endif

	mOverlaySystem = new Ogre::OverlaySystem();
	mFileSystemLayer->setHomePath(mSolutionPath);
}

bool Application::initialiseRTShaderSystem()
{
	if (Ogre::RTShader::ShaderGenerator::initialize())
	{
		mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
		if (mRTShaderLibPath.empty()) return false;

		if (!mMaterialMgrListener) 
		{
			mMaterialMgrListener = new OgreBites::SGTechniqueResolverListener(mShaderGenerator);
			Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
		}
	}

	return true;
}

void Application::destroyRTShaderSystem()
{
	Ogre::MaterialManager::getSingleton().setActiveScheme(Ogre::MaterialManager::DEFAULT_SCHEME_NAME);

	if (mMaterialMgrListener)
	{
		Ogre::MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
		delete mMaterialMgrListener; mMaterialMgrListener = nullptr;
	}

	if (mShaderGenerator)
	{
		Ogre::RTShader::ShaderGenerator::destroy();
		mShaderGenerator = nullptr;
	}
}

void Application::setup()
{
	if (!mRoot->restoreConfig())
	{
		mRoot->showConfigDialog(OgreBites::getNativeConfigDialog());
	}
	else
	{
		mRoot->initialise(false);
		createWindow(mAppName);
		setWindowGrab(false);
		mRoot->addFrameListener(this);

		locateResources();
		initialiseRTShaderSystem();

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		mRoot->addFrameListener(this);
	}
}

void Application::createWindow(const Ogre::String & windowName)
{
	// Params for the Window
	Uint32 w, h;
	Uint32 flags;
	Ogre::String token;
	Ogre::NameValuePairList miscParams;

	// Read the ogre.cfg file
	Ogre::ConfigOptionMap ropts = mRoot->getRenderSystem()->getConfigOptions();
	miscParams["FSAA"] = ropts["FSAA"].currentValue;
	miscParams["vsync"] = ropts["VSync"].currentValue;
	miscParams["gamma"] = ropts["sRGB Gamma Conversion"].currentValue;
	std::istringstream mode(ropts["Video Mode"].currentValue);
	mode >> w >> token >> h;
	if (ropts["Full Screen"].currentValue == "Yes") flags = SDL_WINDOW_FULLSCREEN;
	else flags = SDL_WINDOW_RESIZABLE;

	// Create the SDL Window
	if (!SDL_WasInit(SDL_INIT_VIDEO)) SDL_InitSubSystem(SDL_INIT_VIDEO);
	mWindow.native = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);

	// Create the Render Window
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(mWindow.native, &wmInfo);
	miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));
	mWindow.render = mRoot->createRenderWindow(windowName, w, h, false, &miscParams);
}

void Application::setWindowGrab(bool grab)
{
	SDL_SetWindowGrab(mWindow.native, SDL_bool(grab));
	SDL_ShowCursor(false);
}

void Application::locateResources()
{
	// load resource paths from config file
	Ogre::ConfigFile cf;

	Ogre::String resourcesPath = mFileSystemLayer->getConfigFilePath("resources.cfg");
	if (Ogre::FileSystemLayer::fileExists(resourcesPath))
	{
		cf.load(resourcesPath);
	}
	else
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
			Ogre::FileSystemLayer::resolveBundlePath(mSolutionPath + "\\Assets"),
			"FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}

	Ogre::String sec, type, arch;
	// go through all specified resource groups
	Ogre::ConfigFile::SettingsBySection_::const_iterator seci;
	for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci) {
		sec = seci->first;
		const Ogre::ConfigFile::SettingsMultiMap& settings = seci->second;
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

		// go through all resource paths
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
}

// INPUT HANDLING-------------------------------------------------------------
void Application::pollEvents()
{
	if (!mWindow.native) return;  // SDL events not initialized

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mRoot->queueEndRendering();
			break;

		case SDL_WINDOWEVENT:
			if (event.window.windowID == SDL_GetWindowID(mWindow.native)) 
			{
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					Ogre::RenderWindow* win = mWindow.render;
					windowResized(win);
				}
			}
			break;

		default:
			_fireInputEvent(convert(event));
			break;
		}
	}

	// just avoid "window not responding"
	OgreBites::WindowEventUtilities::messagePump();
}

bool Application::frameRenderingQueued(const Ogre::FrameEvent & evt)
{
	for (OgreBites::InputListener* it : mInputListeners)
	{
		it->frameRendered(evt);
	}

	return true;
}

void Application::_fireInputEvent(const OgreBites::Event & event) const
{
	for (OgreBites::InputListener* l : mInputListeners)
	{
		switch (event.type)
		{
		case OgreBites::KEYDOWN:
			l->keyPressed(event.key);
			break;

		case OgreBites::KEYUP:
			l->keyReleased(event.key);
			break;

		case OgreBites::MOUSEBUTTONDOWN:
			l->mousePressed(event.button);
			break;

		case OgreBites::MOUSEBUTTONUP:
			l->mouseReleased(event.button);
			break;

		case OgreBites::MOUSEWHEEL:
			l->mouseWheelRolled(event.wheel);
			break;

		case OgreBites::MOUSEMOTION:
			l->mouseMoved(event.motion);
			break;
		}
	}
}

// CLOSE APP------------------------------------------------------------------
void Application::close()
{
	if (mRoot != nullptr) 
		mRoot->saveConfig();

	shutdown();

	mRoot = nullptr;
}

void Application::shutdown()
{
	destroyRTShaderSystem();
	delete mWindow.render;
	mWindow.render = nullptr;
	delete mOverlaySystem;
	mOverlaySystem = nullptr;
	SDL_DestroyWindow(mWindow.native);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	mWindow.native = nullptr;
}