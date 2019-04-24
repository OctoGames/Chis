#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "EntityComponentManager.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "UIManager.h"
#include "Physics.h"
#include "SceneLoader.h"
#include "GraphGenerator.h"

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	void createScene();
	void updateScene();

	bool sceneCreated_;
};

#endif // !__APPLICATION_H__