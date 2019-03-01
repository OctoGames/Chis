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
#include "CheckML.h"

//For test
#include "MainCharacter.h"

#include "OgreSystem.h"

class Application
{
private:
	static Application* instance_;

	Application(std::string appname);
	~Application();

	//Method to initialize the mainCamera
	void initCamera();

	//Initializes scene Lights
	void initLights();

	//Pointer to the window, sceneManager and viewport
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* sManager;
	Ogre::Viewport* viewport;

	//Poninter to the main camera and the layer
	Ogre::Camera* mainCamera;

	//Scene nodes to the camera and lights
	Ogre::SceneNode* mCamNode = nullptr;
	Ogre::SceneNode* mLightNode = nullptr;

	//Ground node
	Ogre::SceneNode* mGroundNode;

	//Character node
	MainCharacter* mainCharacter;

	//This method and variable are just a test to see everything works fine
	void createEntity();

	OgreSystem* ogreSystem_ = nullptr;

public:
	static Application* Instance();

	void render();
	SDL_Event handleInput();

	inline Ogre::SceneManager* getSceneManager() { return ogreSystem_->getSceneManager(); }
};

