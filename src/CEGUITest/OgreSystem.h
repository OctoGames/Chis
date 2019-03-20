#ifndef _OGRE_SYSTEM_H_
#define _OGRE_SYSTEM_H_

#include <iostream>

#include "Window.h"

#include <OgreRoot.h>
#include <OgreTimer.h>
#include <OgreLogManager.h>
#include <OgreSceneManager.h>
#include <OgreFileSystemLayer.h>

#include <OgreSceneNode.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreLight.h>
#include <OgreEntity.h>

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISJoyStick.h>
#include <OISInputManager.h>

#include <CEGUI/CEGUI.h>
#include <Renderer.h>

class OgreSystem
{
public:
	static OgreSystem* Instance();

	void init();
	void close();

	inline Ogre::Root* getRoot() { return root_; }
	inline Ogre::SceneManager* getSceneManager() { return sceneManager_; }
	inline Ogre::RenderWindow* getWindow() { return window_->getRenderWindow(); }
	inline OIS::Mouse* getMouse() { return mouse_; }
	inline OIS::Keyboard* getKeyboard() { return keyboard_; }

	inline bool isRunning() const { return running_; }
	void quit() { running_ = false; }

private:
	OgreSystem();

	void createRoot();
	void createWindow();
	void createResources();
	void createSceneManager();
	void createInputManager();

	static OgreSystem* instance_;

	Window* window_;
	Ogre::Root* root_;
	Ogre::String appName_;
	Ogre::String solutionPath_;
	Ogre::SceneManager* sceneManager_;
	Ogre::FileSystemLayer* fileSystemLayer_;

	OIS::Mouse* mouse_;
	OIS::Keyboard* keyboard_;
	OIS::InputManager* inputManager_;

	CEGUI::OgreRenderer* renderer_;
	bool running_;
};

#endif // !_OGRE_SYSTEM_H_
