#include "Application.h"
#include "entitycomponentmanager.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include <string>
#include <SDL.h>

//Garbage collector
#include "CheckML.h"

int main(int argc, char* argv[])
{
	//Garbage collector
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Define the GO
	GameObject* mouse = new GameObject("mouse", "enemy");
	// Define the Transform component
	Transform* mouseTransform_ = new Transform(mouse);
	mouseTransform_->setPosition(0.0, 20.0, 0.0);
	mouseTransform_->setScale(30.0, 30.0, 30.0);

	// Add the GO to the maps
	EntityComponentManager::Instance()->addComponent(mouseTransform_);
	EntityComponentManager::Instance()->addGameObjectWithTag(mouse, mouse->getTag());
	// do after the transform
	// Define the Renderer component
	MeshRenderer* mouseRenderer_ = new MeshRenderer(mouse, "mouse.mesh");
	EntityComponentManager::Instance()->addComponent(mouseRenderer_);

	mouseRenderer_->setMaterialName("mouse_mat");

	while (true)
	{
		//If the application input is Close, we'll quit
		if (Application::Instance()->handleInput().type == SDL_QUIT)
		{
			break;
		}
		Application::Instance()->render();
	}

	SDL_Quit(); //Make sure we destroy SDL window

	return 0;
}