#include "Application.h"


Application::Application(std::string appName) : appName_(appName), mRoot(0)
{
#if _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	mRoot = new Ogre::Root(mPluginsCfg);

	mRoot->setRenderSystem(*(mRoot->getAvailableRenderers().begin()));
	mRoot->initialise(false);

	//Window
	mWindow = mRoot->createRenderWindow(appName_, 800, 600, false);

	mWindow->setActive(true);
	mWindow->setAutoUpdated(true);
	mWindow->setDeactivateOnFocusChange(false);

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

