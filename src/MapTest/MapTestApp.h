#ifndef __TESTAPP_H__
#define __TESTAPP_H__

#include "Application.h"

// OGRE Includes
#include <OgreTrays.h>
#include <OgreCameraMan.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include "DotSceneLoader.h"

class MapTestApp : public Application, OgreBites::InputListener
{
public:
	MapTestApp(const Ogre::String& appname);
	virtual ~MapTestApp();

protected:
	virtual void setup();
	virtual void shutdown();

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);

private:
	virtual void setupScene();

protected:
	// Ogre Managers
	Ogre::SceneManager* mSceneManager;
	OgreBites::CameraMan* mCamManager;
	OgreBites::TrayManager* mTrayManager;

	// Scene elements
	Ogre::SceneNode* mCamNode;
	Ogre::SceneNode* mLightNode;
	Ogre::SceneNode* mLightNode2;
	Ogre::SceneNode* mMouseNode;
	Ogre::SceneNode* mGunNode;
	Ogre::SceneNode* mGroundNode;
	Ogre::SceneNode* mMapNode;
};

#endif