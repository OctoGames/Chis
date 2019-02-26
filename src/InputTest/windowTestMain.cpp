#include "Application.h"
#include <string>

int main()
{
	std::string appName = "CHIS";

	Application* chisApp = new Application(appName);

	delete chisApp;

	return 0;
}