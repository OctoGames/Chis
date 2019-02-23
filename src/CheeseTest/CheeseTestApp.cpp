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
	mMouseNode(nullptr),
	mGroundNode(nullptr)
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

	case SDLK_q:
		mCamNode->rotate(Ogre::Vector3(0, 1, 0), Ogre::Radian(Ogre::Degree(45.0f)), Ogre::Node::TS_WORLD);
		break;

	case SDLK_e:
		mCamNode->rotate(Ogre::Vector3(0, 1, 0), Ogre::Radian(Ogre::Degree(-45.0f)), Ogre::Node::TS_WORLD);
		break;
	}

	return true;
}

void CheeseTestApp::setupScene(void)
{
	// CAMERA------------------------------------------------------------------

	Ogre::Camera* cam = mSceneManager->createCamera("MainCamera");
	cam->setNearClipDistance(0.01);
	cam->setFarClipDistance(10000);
	cam->setAutoAspectRatio(true);

	Ogre::Viewport* vp = getRenderWindow()->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(0.51, 0.41, 0.21));

	mCamNode = mSceneManager->getRootSceneNode()->createChildSceneNode("nMainCamera");
	mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
	mCamNode->setDirection(Ogre::Vector3(0, 0, -1));
	mCamNode->setPosition(0, 100, 1000);
	mCamNode->attachObject(cam);  

	mCamManager = new OgreBites::CameraMan(mCamNode);
	mCamManager->setStyle(OgreBites::CS_FREELOOK);
	addInputListener(mCamManager);


	// LIGHTS------------------------------------------------------------------

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


	// ENTITIES----------------------------------------------------------------

	Ogre::MeshManager::getSingleton().createPlane("GroundPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 10000, 10000, 100, 80, true, 1, 10.0, 10.0, (Ogre::Vector3::NEGATIVE_UNIT_Z));
	Ogre::Entity* ground_ent = mSceneManager->createEntity("GroundPlane");
	ground_ent->setMaterialName("ground_mat");
	mGroundNode = mSceneManager->getRootSceneNode()->createChildSceneNode("nGround");
	mGroundNode->attachObject(ground_ent);

	Ogre::Entity* mouse_ent = mSceneManager->createEntity("mouse.mesh");
	mouse_ent->setMaterialName("mouse_mat");
	mMouseNode = mSceneManager->getSceneNode("nGround")->createChildSceneNode("nMouse");
	mMouseNode->setPosition(Ogre::Vector3(0, 25, 0));
	mMouseNode->setScale(40, 40, 40);
	mMouseNode->attachObject(mouse_ent);

	Ogre::Entity* gun_ent = mSceneManager->createEntity("gun.mesh");
	gun_ent->setMaterialName("gun_mat");
	mGunNode = mSceneManager->getSceneNode("nMainCamera")->createChildSceneNode("nGun");
	mGunNode->rotate(Ogre::Vector3(0, 1, 0), Ogre::Radian(Ogre::Degree(180.0f)));
	mGunNode->setPosition(Ogre::Vector3(30, -50, -90));
	mGunNode->attachObject(gun_ent);

	mSceneManager->setSkyDome(true, "skyPlane");
	//mSceneManager->setSkyPlane(true, Ogre::Plane(Ogre::Vector3::UNIT_Z, -50), "skyPlane", 1, 1, true, 1.0, 100, 100);
}
