#pragma once

//Garbage collector
//#include "CheckML.h"

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	void initSystems();
	void initEntities();

	void handleInput();
	void update();
	void render();

	bool running_;
};

