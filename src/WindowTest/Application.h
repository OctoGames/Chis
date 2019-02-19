#pragma once

#include "OgreRoot.h"
#include <OgreConfigFile.h>
#include "OgreRenderWindow.h"
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreFileSystemLayer.h>
#include <string>
#include <fstream>
#include <iostream>

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

	void readDataFromFile();
	std::string readString(std::string s);

	bool fullScreen_;
	int winWidth_;
	int winHeight_;

public:
	Application(std::string appname);
	~Application();
};

