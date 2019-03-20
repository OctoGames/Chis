#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "OgreSystem.h"

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	void initSystems();
	void closeDemo();
	void closeSystems();
	void initDemo();

	void handleInput(float deltaTime);
	void update();
	void render();

	Ogre::SceneNode* cameraNode_;
	Ogre::Viewport* viewport_;
};

#endif // !_APPLICATION_H_

