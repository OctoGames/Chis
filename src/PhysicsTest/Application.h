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
#include <OgreResourceGroupManager.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreMeshManager.h>

//SDL includes
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_syswm.h>

//Garbage collector
//#include "CheckML.h"

//For test
#include "MainCharacter.h"

#include "Physics.h"



class Application
{

private:

	//Method to load plugins
	void loadPluggins();

	//Method to setUp the mainwindow
	void setWindow();

	//Method to initialize the mainCamera
	void initCamera();

	//Initializes scene Lights
	void initLights();

	std::string appName_; //Application name

	Ogre::Root* mRoot; //Ogre Root

	//Names of the config files and paths to take all resources needes as well as plugins
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::String resourcesPath;

	//Pointer to the window, sceneManager and viewport
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* sManager;
	Ogre::Viewport* viewport;

	//Poninter to the main camera and the layer
	Ogre::Camera* mainCamera;
	Ogre::FileSystemLayer* mFSLayer;

	//Method that read a config file line by line
	void readDataFromFile();
	//this one recieves a string and takes the whole right part after the first "=" sing
	std::string readString(std::string s);
	std::string findConfig(std::string config, std::string configFilePath);

	//Scene nodes to the camera and lights
	Ogre::SceneNode* mCamNode = nullptr;
	Ogre::SceneNode* mLightNode = nullptr;

	//Ground node
	Ogre::SceneNode* mGroundNode;

	//Character node
	MainCharacter* mainCharacter;

	//Variables to control screen needs
	bool fullScreen_;
	int winWidth_;
	int winHeight_;

	//Methods to load and set resources 
	void setUpResources();
	void loadResources();

	//This method and variable are just a test to see everything works fine
	void createEntity();

	Physics* physic;
public:

	void render();
	void update();
	SDL_Event handleInput();
	Application(std::string appname);
	~Application();
};

