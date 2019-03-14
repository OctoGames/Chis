#include "Application.h"

#include "OgreSystem.h"
#include "EntityComponentManager.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "AudioSource.h"
#include "Physics.h"

Application::Application() :
	running_(true)
{
	initSystems();
	initDemo();
}

Application::~Application()
{
	closeSystems();
}

void Application::initSystems()
{
	OgreSystem::Instance()->init();
	Physics::Instance()->setDebugMode(true);
}

void Application::closeSystems()
{
	OgreSystem::Instance()->close();
	EntityComponentManager::Instance()->close();
}

void Application::initDemo()
{
	//------------------LIGHTS-------------------//
	GameObject* mainLight = new GameObject("light", "mainLight");
	Transform* mainLightTransform = new Transform(mainLight);
	Light* mainLight_ = new Light(mainLight, true);
	mainLightTransform->setPosition(0, 0, 20);

	//------------------CAMERA------------------//
	GameObject* cam = new GameObject("camera", "cam");
	Transform* cameraTransform = new Transform(cam);
	Camera* mainCamera = new Camera(cam, true);
	cameraTransform->setPosition(0, 0, 200);
	AudioSource* bgMusic = new AudioSource(cam, "MouseMusic", "22-The Mouse's House.mp3");
	bgMusic->play();

	//------------------ACTION------------------//
	btVector3 mouseScale(60, 30, 30);
	GameObject* mouse = new GameObject("mouse", "enemy");
	Transform* mouseTransform_ = new Transform(mouse);
	mouseTransform_->setPosition(0.0, 20.0, 0.0);
	mouseTransform_->setScale(30.0, 30.0, 30.0);
	MeshRenderer* mouseRenderer_ = new MeshRenderer(mouse, "mouse.mesh");
	mouseRenderer_->setMaterialName("mouse_mat");
	Physics::Instance()->createRigidBody(mouseTransform_->getNode(), 1, mouseScale, "mouseRB");
}

void Application::run()
{
	while (isRunning())
	{
		handleInput();
		update();
		render();
	}
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