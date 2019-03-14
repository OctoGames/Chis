#include "Application.h"
#include "entitycomponentmanager.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "AudioSource.h"
#include "Physics.h"

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
	OgreSystem::Instance()->init("CHIS");
}

void Application::initEntities()
{

	//----------------MOUSE OBJECT------------------//
	btVector3 mouseScale(60, 30, 30);

	GameObject* mouse = new GameObject("mouse", "enemy");
	Transform* mouseTransform_ = new Transform(mouse);
	mouseTransform_->setPosition(0.0, 100.0, 0.0);
	mouseTransform_->setScale(30.0, 30.0, 30.0);
	MeshRenderer* mouseRenderer_ = new MeshRenderer(mouse, "mouse.mesh");
	mouseRenderer_->setMaterialName("mouse_mat");

	Physics::Instance()->createRigidBody(mouseTransform_->getNode(), 1, mouseScale, "mouseRB");

	//----------------FLOOR OBJECT------------------// ONLY FOR TEST NOT WITH COMPONENTS

	btVector3 floorScale(1000, 10, 1000);

	//Create Ground
	Ogre::MeshManager::getSingleton().createPlane("GroundPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 10000, 10000, 100, 80, true, 1, 10.0, 10.0, (Ogre::Vector3::NEGATIVE_UNIT_Z));

	Ogre::Entity* ground_ent = OgreSystem::Instance()->getSceneManager()->createEntity("GroundPlane");
	ground_ent->setMaterialName("ground_mat");

	Ogre::SceneNode* mGroundNode = OgreSystem::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("nGround");
	mGroundNode->attachObject(ground_ent);

	Physics::Instance()->createRigidBody(mGroundNode, 0, floorScale, "floorRB");

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
	EntityComponentManager::Instance()->tick();
	AudioSystem::Instance()->update();
}

void Application::render()
{
	OgreSystem::Instance()->getRoot()->renderOneFrame();
}