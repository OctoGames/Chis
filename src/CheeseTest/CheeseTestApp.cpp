#include "CheeseTestApp.h"

#include <OgreEntity.h>
#include <OgreInput.h>
#include <SDL_keycode.h>
#include <OgreMeshManager.h>

CheeseTestApp::CheeseTestApp(const Ogre::String& appname) :
	Application(appname),
	mSceneManager(nullptr),
	mCamManager(nullptr),
	mTrayManager(nullptr),
	mCamNode(nullptr),
	mLightNode(nullptr),
	mSinbadNode(nullptr)
{
}

CheeseTestApp::~CheeseTestApp()
{
}

void CheeseTestApp::setup(void)
{
	Application::setup();

	mSceneManager = mRoot->createSceneManager();
	mShaderGenerator->addSceneManager(mSceneManager);
	mSceneManager->addRenderQueueListener(mOverlaySystem);

	mTrayManager = new OgreBites::TrayManager("TrayGUISystem", mWindow.render);
	mTrayManager->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	addInputListener(mTrayManager);

	addInputListener(this);
	setupScene();
}

void CheeseTestApp::shutdown()
{
	mShaderGenerator->removeSceneManager(mSceneManager);
	mSceneManager->removeRenderQueueListener(mOverlaySystem);
	mRoot->destroySceneManager(mSceneManager);

	delete mTrayManager;  mTrayManager = nullptr;
	delete mCamManager; mCamManager = nullptr;

	Application::shutdown();
}

bool CheeseTestApp::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	switch (evt.keysym.sym)
	{
	case SDLK_ESCAPE:
		getRoot()->queueEndRendering();
		break;
	}

	return true;
}

void CheeseTestApp::setupScene(void)
{
	Ogre::Camera* cam = mSceneManager->createCamera("MainCamera");
	cam->setNearClipDistance(1);
	cam->setFarClipDistance(10000);
	cam->setAutoAspectRatio(true);

	Ogre::Viewport* vp = getRenderWindow()->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(0.51, 0.41, 0.21));

	mCamNode = mSceneManager->getRootSceneNode()->createChildSceneNode("nMainCamera");
	mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
	mCamNode->setDirection(Ogre::Vector3(0, 0, -1));
	mCamNode->setPosition(0, 0, 1000);
	mCamNode->attachObject(cam);  

	//------------------------------------------------------------------------

	Ogre::Light* light = mSceneManager->createLight("Light");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDiffuseColour(1, 1, 1);

	Ogre::Light* light2 = mSceneManager->createLight("Light2");
	light2->setType(Ogre::Light::LT_DIRECTIONAL);
	light2->setDiffuseColour(1, 1, 1);

	mLightNode = mCamNode->createChildSceneNode("nLight");
	mLightNode->setDirection(Ogre::Vector3(0, 0, -1));
	mLightNode->attachObject(light);

	mLightNode2 = mSceneManager->getRootSceneNode()->createChildSceneNode("nLight2");
	mLightNode2->setPosition(Ogre::Vector3(0, 20, 0));
	mLightNode2->setDirection(Ogre::Vector3(0, -1, 0));
	mLightNode2->attachObject(light2);

	//------------------------------------------------------------------------

	Ogre::Entity* ent = mSceneManager->createEntity("mesh_id32.mesh");

	mSinbadNode = mSceneManager->getRootSceneNode()->createChildSceneNode("nSinbad");
	mSinbadNode->setScale(40, 40, 40);
	mSinbadNode->attachObject(ent);

	//------------------------------------------------------------------------

	mCamManager = new OgreBites::CameraMan(mCamNode);
	mCamManager->setStyle(OgreBites::CS_ORBIT);
	addInputListener(mCamManager);
}
