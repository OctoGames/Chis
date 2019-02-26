#include "Application.h"
#include <string>
#include <SDL.h>

int main(int argc, char* argv[])
{
	std::string appName = "CHIS";

	Application* chisApp = new Application(appName);

	SDL_Event evt;

	while (true)
	{
		//If the application input is Close, we'll quit
		if (chisApp->handleInput().type == SDL_QUIT)
		{
			break;
		}
		chisApp->render();
	}

	SDL_Quit(); //Make sure we destroy SDL window

	delete chisApp;

	return 0;
}