#ifndef __TESTAPP_H__
#define __TESTAPP_H__

#include "Application.h"

// OGRE Includes
#include <OgreTrays.h>
#include <OgreCameraMan.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

class CheeseTestApp : public Application, OgreBites::InputListener
{
public:
	CheeseTestApp(const Ogre::String& appname);
	virtual ~CheeseTestApp();

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
	Ogre::SceneNode* mSinbadNode;
};

#endif