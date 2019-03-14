#include "Application.h"

#include "OgreSystem.h"

Application::Application() :
	running_(true)
{
	OgreSystem::Instance()->init();
}

Application::~Application()
{
	OgreSystem::Instance()->close();
}

void Application::run()
{
	while (isRunning())
	{
		handleInput();
		update();
		render();
	}
}

void Application::handleInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			running_ = false;
		}

		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				running_ = false;
			}
		}
	}
}

void Application::update()
{

}

void Application::render()
{
	OgreSystem::Instance()->getRoot()->renderOneFrame();
}