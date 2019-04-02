#include "Application.h"

#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "FirstPersonCamera.h"
#include "Light.h"
#include "AudioSource.h"
#include "RigidBody.h"

Application::Application()
{
	RenderManager::Instance()->init();
	InputManager::Instance()->init();
	UIManager::Instance()->init();
	createScene();
}

Application::~Application()
{
	UIManager::Instance()->close();
	InputManager::Instance()->close();
	RenderManager::Instance()->close();
}

void Application::run()
{
	while (RenderManager::Instance()->isRunning())
	{
		float dt = RenderManager::Instance()->time()->tick();
		if (dt == 0.0f) continue;

		AudioSystem::Instance()->update();
		InputManager::Instance()->update(dt);
		updateScene();
		Physics::Instance()->update();
		EntityComponentManager::Instance()->update();
		RenderManager::Instance()->update(dt);
	}
}

void Application::createScene()
{
	//-----------------READ SCENE-------------------//
	SceneLoader::Instance()->loadScene("test.scene");
	//GraphGenerator* graphG = new GraphGenerator();
	//graphG->initG();
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
	FirstPersonCamera* mainCamera = new FirstPersonCamera(cam, true);
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

void Application::updateScene()
{
	if (InputManager::Instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE))
	{
		RenderManager::Instance()->setRunning(false);
	}
	else if (InputManager::Instance()->getKeyboard()->isKeyDown(OIS::KC_P))
	{
		Physics::Instance()->toggleDebug();
	}
	else if (InputManager::Instance()->getKeyboard()->isKeyDown(OIS::KC_O))
	{
		Physics::Instance()->toggleDebugMode();
	}
}