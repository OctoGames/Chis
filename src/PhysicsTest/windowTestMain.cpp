#include "Application.h"
#include <string>
#include <SDL.h>


//Garbage collector
#include "CheckML.h"

int main(int argc, char* argv[])
{
	//Garbage collector
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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