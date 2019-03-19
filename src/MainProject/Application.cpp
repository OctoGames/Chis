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
	mouseRigidBody->createBoxRB(1, Ogre::Vector3(50,30,30), "mouseRB");

	MeshRenderer* mouseRenderer_ = new MeshRenderer(mouse, "mouse.mesh");
	mouseRenderer_->setMaterialName("mouseMaterial");

	//------------------MAIN CAMERA------------------//

	GameObject* cam = new GameObject("camera", "cam");
	Transform* cameraTransform = new Transform(cam);
	Camera* mainCamera = new Camera(cam, true);
	cameraTransform->setPosition(200, 100, 400);
	cameraTransform->getNode()->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
	AudioSource* bgMusic = new AudioSource(cam, "MouseMusic", "22-The Mouse's House.mp3");
	bgMusic->play();

	//------------------LIGHTS-----------------------//

	GameObject* mainLight = new GameObject("light", "mainLight");
	Transform* mainLightTransform = new Transform(mainLight);
	Light* mainLight_ = new Light(mainLight, true);
	mainLightTransform->setPosition(0, 20, 0);
	mainLightTransform->getNode()->setDirection(Ogre::Vector3(0, -1, -1));
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
				Physics::Instance()->toggleDebug();
			}

			else if (event.key.keysym.sym == SDLK_o)
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