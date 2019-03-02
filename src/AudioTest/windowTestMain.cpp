#include "Application.h"
#include <string>
#include <SDL.h>
#include <fmod.hpp>
#include <fmod_errors.h>
using namespace FMOD;


//Garbage collector
#include "CheckML.h"

int main(int argc, char* argv[])
{
	//Garbage collector
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::string appName = "CHIS";

	Application* chisApp = new Application(appName);

	SDL_Event evt;

	//FMOD::System_Create;

	//FMOD_RESULT result;
	//FMOD::System *system = NULL;

	//std::cout << "FMOD Init"<< std::endl;
	//result = FMOD::System_Create(&system);      // Create the main system object.
	//if (result != FMOD_OK) {
	//	std::cout << "FMOD error: " << FMOD_ErrorString(result) << std::endl;
	//	 printf("FMOD error %d - %s", result, FMOD_ErrorString(result));
	//	exit(-1);

	//}

	//result = system->init(128, FMOD_INIT_NORMAL, 0); // Inicializacion de FMOD
	//if (result != FMOD_OK) {
	//	std::cout << "FMOD error: "<< FMOD_ErrorString(result) << std::endl;
	//	 printf("FMOD error %d - %s", result, FMOD_ErrorString(result));
	//	exit(-1);
	//}



	//Sound *sound1;
	//std::cout << "FMOD playing" << std::endl;

	//result = system->createSound("caca", FMOD_DEFAULT, 0, &sound1);
	// path al archivo de sonido, valores (por defecto en este caso: sin loop, 2D), informacion adicional (nada en este caso)

	//Channel *channel;
	//result = system->playSound(sound1, 0, false, &channel);




	while (true)
	{
		//If the application input is Close, we'll quit
		if (chisApp->handleInput().type == SDL_QUIT)
		{
			break;
		}
		//result = system->update();
		chisApp->render();
	}

	SDL_Quit(); //Make sure we destroy SDL window

	delete chisApp;

	return 0;
}