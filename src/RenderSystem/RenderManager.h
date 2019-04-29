#ifndef __RENDER_MANAGER_H__
#define __RENDER_MANAGER_H__

#include <iostream>

#include <Ogre.h>
#include <OgreRoot.h>
#include <OgreLogManager.h>
#include <OgreSceneManager.h>
#include <OgreFileSystemLayer.h>

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreLight.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSkeleton.h>

#include "TimeManager.h"
#include "Window.h"

class RenderManager
{
public:
	static RenderManager* Instance();

	void init();
	void update(float deltaTime);
	void close();

	inline bool isRunning() const { return running_; }
	inline void setRunning(bool running) { running_ = running; }

	inline TimeManager* time() { return time_; }
	inline Ogre::Root* getRoot() { return root_; }
	inline Ogre::SceneManager* getSceneManager() { return sceneManager_; }
	inline SDL_Window* getNativeWindow() { return window_->getNativeWindow(); }
	inline Ogre::RenderWindow* getWindow() { return window_->getRenderWindow(); }

private:
	RenderManager();

	void initRoot();
	void loadResources();

	static RenderManager* instance_;

	TimeManager* time_;
	Window* window_;
	bool running_;

	Ogre::String appName_;
	Ogre::String pluginsCfg_;
	Ogre::String resourcesCfg_;
	Ogre::String solutionPath_;

	Ogre::Root* root_;
	Ogre::SceneManager* sceneManager_;
	Ogre::FileSystemLayer* fileSystemLayer_;
};

#endif // !__RENDER_MANAGER_H__