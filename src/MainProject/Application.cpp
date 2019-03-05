#include "Application.h"

Application* Application::instance_ = nullptr;

Application::Application(std::string appName)
{
	ogreSystem_ = new OgreSystem("Chis");
}

Application::~Application()
{

}

void Application::render()
{
	ogreSystem_->getRoot()->renderOneFrame();
}

Application * Application::Instance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Application("CHIS");
	}
	return instance_;
}

//This method will handle the input from SDL and return the event taken
SDL_Event Application::handleInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				event.type = SDL_QUIT;

			}
		}
	}

	return event;

}