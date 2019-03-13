#include <string>

#include "Application.h"
#include "entitycomponentmanager.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "AudioSource.h"
#include "Physics.h"

//Garbage collector
//#include "CheckML.h"

int main(int argc, char* argv[])
{
	//Garbage collector
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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

	//------------------MAIN CAMERA------------------//

	//Create main Camera
	GameObject* cam = new GameObject("camera", "cam");
	Transform* cameraTransform = new Transform(cam);
	// Add the GO to the maps
	EntityComponentManager::Instance()->addComponent(cameraTransform);
	EntityComponentManager::Instance()->addGameObjectWithTag(cam, cam->getTag());

	Camera* mainCamera = new Camera(cam, true);

	EntityComponentManager::Instance()->addComponent(mainCamera);

	cameraTransform->setPosition(0, 0, 200);

	AudioSource* bgMusic = new AudioSource(cam, "MouseMusic", "22-The Mouse's House.mp3");
	EntityComponentManager::Instance()->addComponent(bgMusic);
	bgMusic->play();

	//------------------LIGHTS-----------------------//
	//Create main Light
	GameObject* mainLight = new GameObject("light", "mainLight");
	Transform* mainLightTransform = new Transform(mainLight);

	// Add the GO to the maps
	EntityComponentManager::Instance()->addComponent(mainLightTransform);
	EntityComponentManager::Instance()->addGameObjectWithTag(mainLight, mainLight->getTag());

	Light* mainLight_ = new Light(mainLight, true);

	EntityComponentManager::Instance()->addComponent(mainLight_);

	mainLightTransform->setPosition(0, 0, 20);

	Physics::Instance()->createRigidBody(mouseTransform_->getNode(), 1, "mouseRB");

	while (Application::Instance()->handleInput().type != SDL_QUIT)
	{
		EntityComponentManager::Instance()->tick();
		AudioSystem::Instance()->update();
		Application::Instance()->render();
		Physics::Instance()->update();
	}

	SDL_Quit(); //Make sure we destroy SDL window

	return 0;
}