#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "RenderManager.h"
#include "InputManager.h"
#include "FirstPersonCamera.h"

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	void createScene();
	void updateScene(float deltaTime);

	FirstPersonCamera* fpsCamera_;
};

#endif // !__APPLICATION_H__

