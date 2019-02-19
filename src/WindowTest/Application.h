#pragma once

#include "OgreRoot.h"
#include <OgreConfigFile.h>
#include "OgreRenderWindow.h"
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreFileSystemLayer.h>
#include <string>

class Application
{

private:

	std::string appName_;
	Ogre::Root* mRoot; //Ogre Root

	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;

	Ogre::RenderWindow* mWindow;

	Ogre::SceneManager* sManager;

	Ogre::Viewport* viewport;

public:
	Application(std::string appName);
	~Application();
};

