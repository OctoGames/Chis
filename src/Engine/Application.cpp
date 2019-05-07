#include "Application.h"

#include "Engine.h"

Application::Application()
{
}

Application::~Application()
{
}

void Application::init()
{
	RenderManager::Instance()->init();
	InputManager::Instance()->init();
	GUIManager::Instance()->init();
	Physics::Instance()->init();
}

void Application::run()
{
	while (RenderManager::Instance()->isRunning())
	{
		float dt = RenderManager::Instance()->time()->tick();
		if (dt == 0.0f) continue;

		AudioSystem::Instance()->update();
		InputManager::Instance()->update(dt);
		Physics::Instance()->update(dt);
		EntityComponentManager::Instance()->update();
		GUIManager::Instance()->update(dt);
		RenderManager::Instance()->update(dt);
	}
}

void Application::close()
{
	Physics::Instance()->close();
	GUIManager::Instance()->close();
	InputManager::Instance()->close();
	RenderManager::Instance()->close();
}