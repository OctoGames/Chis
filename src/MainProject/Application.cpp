#include "Application.h"
#include "EntityComponentManager.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "AudioSource.h"
#include "RigidBody.h"
#include "Physics.h"
#include "SceneLoader.h"

Application::Application() : running_(true)
{
	initSystems();
	initEntities();
}

Application::~Application()
{

}

void Application::run()
{
	while (running_)
	{
		handleInput();
		update();
		render();
	}
}

void Application::initSystems()
{
	OgreSystem::Instance()->init();
}

void Application::initEntities()
{
	//-----------------READ SCENE-------------------//
	SceneLoader::Instance()->loadScene("test.scene");

	//----------------MOUSE OBJECT------------------//

	GameObject* mouse = new GameObject("mouse", "enemy");
	Transform* mouseTransform_ = new Transform(mouse);
	mouseTransform_->setPosition(0.0, 100.0, 0.0);
	mouseTransform_->setScale(30.0, 30.0, 30.0);

	RigidBody* mouseRigidBody = new RigidBody(mouse);
	mouseRigidBody->createSphereRB(1, 30, "mouseRB");

	MeshRenderer* mouseRenderer_ = new MeshRenderer(mouse, "mouse.mesh");
	mouseRenderer_->setMaterialName("mouseMaterial");

	//----------------FLOOR OBJECT------------------//

	Ogre::Vector3 floorScale(1000, 10, 1000);

	GameObject* floor = new GameObject("floor", "floor");
	Transform* floorTransform_ = new Transform(floor);
	floorTransform_->setPosition(0.0, 0.0, 0.0);
	floorTransform_->setScale(1000, 10, 1000);

	RigidBody* floorRigidBody = new RigidBody(floor);
	floorRigidBody->createBoxRB(0, floorScale, "floorRB");

	//Theres a fish mesh as floor because I dont wanna waste time creating a box mesh, obviously should be changed in the future
	MeshRenderer* floorRenderer_ = new MeshRenderer(floor, "fish.mesh");
	floorRenderer_->setMaterialName("ground_mat");

	//------------------MAIN CAMERA------------------//

	GameObject* cam = new GameObject("camera", "cam");
	Transform* cameraTransform = new Transform(cam);
	Camera* mainCamera = new Camera(cam, true);
	cameraTransform->setPosition(0, 50, 400);
	AudioSource* bgMusic = new AudioSource(cam, "MouseMusic", "22-The Mouse's House.mp3");
	bgMusic->play();

	//------------------LIGHTS-----------------------//

	GameObject* mainLight = new GameObject("light", "mainLight");
	Transform* mainLightTransform = new Transform(mainLight);
	Light* mainLight_ = new Light(mainLight, true);
	mainLightTransform->setPosition(0, 20, 0);
	mainLightTransform->getNode()->setDirection(Ogre::Vector3(0, -1, -1));

	Physics::Instance()->setDebugMode(true);
}

void Application::handleInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			running_ = false;
		}

		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				running_ = false;
			}

			else if (event.key.keysym.sym == SDLK_p)
			{
				Physics::Instance()->toggleDebugMode();
			}
		}
	}
}

void Application::update()
{
	Physics::Instance()->update();
	EntityComponentManager::Instance()->update();
	AudioSystem::Instance()->update();
}

void Application::render()
{
	OgreSystem::Instance()->getRoot()->renderOneFrame();
}