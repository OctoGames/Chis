#include <string>
#include <fstream>
#include <iostream>

//Ogre dependencies
#include "OgreRoot.h"
#include <OgreConfigFile.h>
#include "OgreRenderWindow.h"
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
	OgreSystem(std::string appname);
	~OgreSystem() {};

	inline Ogre::Root* getRoot() { return mRoot; };
	inline Ogre::SceneManager* getSceneManager() { return sManager; };

private:
	std::string appName_; //Application name
	Ogre::Root* mRoot; //Ogre Root

	//Variables to control screen needs
	bool fullScreen_;
	int winWidth_;
	int winHeight_;

	//Names of the config files and paths to take all resources needes as well as plugins
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::String resourcesPath;

	//Pointer to the window, sceneManager and viewport
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* sManager;
	//Ogre::Viewport* viewport;

	//Poninter to the main camera and the layer
	//Ogre::Camera* mainCamera;
	Ogre::FileSystemLayer* mFSLayer;

	//Method to load plugins
	void loadPluggins();

	//Method to setUp the mainwindow
	void setWindow();

	//Method that read a config file line by line
	void readDataFromFile();

	//this one recieves a string and takes the whole right part after the first "=" sing
	std::string readString(std::string s);
	std::string findConfig(std::string config, std::string configFilePath);

	//Methods to load and set resources 
	void setUpResources();
	void loadResources();
};