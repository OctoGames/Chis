#include "Application.h"

Application* Application::instance_ = nullptr;

Application::Application(std::string appName)
{
	ogreSystem_ = new OgreSystem("Chis");

	// Initialice the camera
	//initCamera();

	//Init Lights
	//initLights();
}

Application::~Application()
{
	//delete mRoot;

	//delete mWindow;

	//delete sManager;

	//delete viewport;

	//delete mainCamera;
}

void Application::render()
{
	ogreSystem_->getRoot()->renderOneFrame();
}

//Initialize the main camera being a child of the main character
void Application::initCamera()
{
	// viewport and camera
	mainCamera = sManager->createCamera("mainCamera");
	mainCamera->setNearClipDistance(1);
	mainCamera->setFarClipDistance(10000);
	mainCamera->setAutoAspectRatio(true);

	mCamNode = sManager->getRootSceneNode()->createChildSceneNode("nCam");
	mCamNode->attachObject(mainCamera);

	mCamNode->setPosition(0, 0, 1000);
	mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

	viewport = mWindow->addViewport(mainCamera);
	viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.5, 0.5));
}

void Application::initLights()
{
	// without light we would just get a black screen 
	Ogre::Light* luz = sManager->createLight("Luz");
	luz->setType(Ogre::Light::LT_DIRECTIONAL);
	luz->setDiffuseColour(0.75, 0.75, 0.75);

	mLightNode = mCamNode->createChildSceneNode("nLuz");
	mLightNode->attachObject(luz);
	mLightNode->setDirection(Ogre::Vector3(0, -1, -1));
}

Application * Application::Instance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Application("CHIS");
	}
	return instance_;
}

//This method will handle the input from SDL and return the event taken
SDL_Event Application::handleInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				event.type = SDL_QUIT;

			}
		}
	}

	return event;

}