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

	void handleInput(float lDeltaTime_s);
	void update();
	void render();

	Ogre::SceneNode* cameraNode_;
	Ogre::Viewport* viewport_;
};

#endif // !_APPLICATION_H_

