#pragma once

//Ogre includes
#include "Ogre.h"
#include "OgreSystem.h"

//SDL includes
#include <SDL.h>

//Garbage collector
#include "CheckML.h"


class Application
{
private:
	static Application* instance_;

	Application(std::string appname);
	~Application();

	OgreSystem* ogreSystem_ = nullptr;

public:
	static Application* Instance();

	void render();
	SDL_Event handleInput();

	inline Ogre::SceneManager* getSceneManager() { return ogreSystem_->getSceneManager(); }
	inline Ogre::RenderWindow* getWindow() { return ogreSystem_->getWindow(); };
};

