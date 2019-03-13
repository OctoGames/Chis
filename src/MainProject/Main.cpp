#include "Application.h"

//Garbage collector
//#include "CheckML.h"

int main(int argc, char* argv[])
{
	//Garbage collector
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Application* app = new Application();
	app->run();
	delete app;

	return 0;
}