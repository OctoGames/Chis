#ifndef _OGRE_SYSTEM_H_
#define _OGRE_SYSTEM_H_

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreFileSystemLayer.h>

#include "Window.h"

class OgreSystem
{
public:
	static OgreSystem* Instance();

	void init();
	void close();

	inline Ogre::Root* getRoot() { return root_; }
	inline Ogre::SceneManager* getSceneManager() { return sceneManager_; }

private:
	OgreSystem();

	void createRoot();
	void createWindow();
	void createResources();
	void createSceneManager();

	static OgreSystem* instance_;

	Window* window_;
	Ogre::Root* root_;
	Ogre::SceneManager* sceneManager_;
	Ogre::FileSystemLayer* fileSystemLayer_;

	Ogre::String appName_;
	Ogre::String solutionPath_;
};


#endif // !_OGRE_SYSTEM_H_
