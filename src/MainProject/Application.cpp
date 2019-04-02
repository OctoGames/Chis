#include "Application.h"

#include "Transform.h"
#include "MeshRenderer.h"
#include "FirstPersonCamera.h"
#include "DirectionalLight.h"
#include "AudioSource.h"
#include "RigidBody.h"

Application::Application()
{
	RenderManager::Instance()->init();
	InputManager::Instance()->init();
	UIManager::Instance()->init();

	EntityComponentManager::Instance()->addFactory("Transform", &TransformFactory());
	EntityComponentManager::Instance()->addFactory("MeshRenderer", &MeshRendererFactory());
	EntityComponentManager::Instance()->addFactory("FirstPersonCamera", &FirstPersonCameraFactory());
	EntityComponentManager::Instance()->addFactory("DirectionalLight", &DirectionalLightFactory());
	EntityComponentManager::Instance()->addFactory("AudioSource", &AudioSourceFactory());
	EntityComponentManager::Instance()->addFactory("Rigidbody", &RigidBodyFactory());

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
	Transform* mainLightTransform = new Transform(mainLight,  "camera");
	DirectionalLight* mainLight_ = new DirectionalLight(mainLight, true);
	//mainLightTransform->setPosition(0, 20, 0);
	mainLightTransform->getNode()->setDirection(Ogre::Vector3(0, 0, -1));

	//std::map<std::string, ValueType> params;
	//params["parent"].s = "";
	//params["position_x"].f = 0.0f;
	//params["position_y"].f = 20.0f;
	//params["position_z"].f = 0.0f;
	//params["direction_x"].f = 0.0f;
	//params["direction_y"].f = -1.0f;
	//params["direction_z"].f = -1.0f;
	//params["scale_x"].f = 1.0f;
	//params["scale_y"].f = 1.0f;
	//params["scale_z"].f = 1.0f;
	//params["enabled"].b = true;
	//
	//BaseFactory* f = EntityComponentManager::Instance()->getFactory("Transform");
	//Component* c1 = f->create();
	//c1->setContainer(mainLight);
	//c1->init(params);

	//params["diffuse_x"].f = 0.75f;
	//params["diffuse_y"].f = 0.75f;
	//params["diffuse_z"].f = 0.75f;
	//params["enabled"].b = true;

	//Component* c2 = EntityComponentManager::Instance()->getFactory("DirectionalLight")->create();
	//c2->setContainer(mainLight);
	//c2->init(params);
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