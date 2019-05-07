#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "EntityComponentManager.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "GUIManager.h"
#include "Physics.h"
#include "SceneLoader.h"
#include "ArchetypeLoader.h"
#include "GraphGenerator.h"

class Application
{
public:
	Application();
	~Application();

	void run();
};

#endif // !__APPLICATION_H__