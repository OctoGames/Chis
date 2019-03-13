#ifndef _OGRE_SYSTEM_H_
#define _OGRE_SYSTEM_H_

#include <string>
#include <fstream>
#include <iostream>

//Ogre dependencies
#include <Ogre.h>
#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreFileSystemLayer.h>
#include <OgreResourceGroupManager.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreMeshManager.h>

//SDL includes
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_syswm.h>

class OgreSystem
{
public:
	void init(std::string appname);
	static OgreSystem* Instance();

	inline Ogre::Root* getRoot() { return root_; };
	inline Ogre::RenderWindow* getWindow() { return window_; };
	inline Ogre::SceneManager* getSceneManager() { return sceneManager_; };

private:
	OgreSystem() {}

	// Loads plugins
	void loadPlugins();

	// Setups the render window
	void setupWindow();

	// Reads a config file line by line
	void readDataFromFile();
	std::string readString(std::string s);
	std::string findConfig(std::string config, std::string configFilePath);

	//Methods to load and set resources 
	void setUpResources();
	void loadResources();

	static OgreSystem* instance_;

	int winWidth_;			// Window width
	int winHeight_;			// Window height
	bool fullScreen_;		// Fullscreen flag
	std::string appName_;	// Application name

	Ogre::Root* root_;							// Ogre Root
	Ogre::String pluginsCfg_;					// Plugins config file
	Ogre::String resourcesCfg_;					// Resources config file
	Ogre::String resourcesPath_;				// Resources relative path
	Ogre::RenderWindow* window_;				// Render window
	Ogre::SceneManager* sceneManager_;			// Ogre scene manager
	Ogre::FileSystemLayer* fileSystemLayer_;	// Ogre File System Layer
};

#endif