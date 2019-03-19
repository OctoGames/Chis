#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "OgreSystem.h"

class Application
{
public:
	Application();
	~Application();

	void run();

	inline bool isRunning() const { return running_; }
	inline void setRunning(bool running) { running_ = running; }

private:
	bool running_;

	void initSystems();
	void closeSystems();
	void initDemo();

	void handleInput(unsigned long lDeltaTime_s);
	void update();
	void render();

	Ogre::SceneNode* lCameraNode;
	Ogre::Viewport* vp;
};

#endif // !_APPLICATION_H_

