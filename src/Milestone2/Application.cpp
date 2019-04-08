#include "Application.h"

#include "Transform.h"
#include "MeshRenderer.h"
#include "FirstPersonCamera.h"
#include "DirectionalLight.h"
#include "AudioSource.h"
#include "RigidBody.h"
#include "GunController.h"

Application::Application() :
	sceneCreated_(false)
{
	RenderManager::Instance()->init();
	InputManager::Instance()->init();
	UIManager::Instance()->init();

	// Register the Component Factories before the Application starts
	EntityComponentManager::Instance()->addFactory("Transform", new TransformFactory());
	EntityComponentManager::Instance()->addFactory("MeshRenderer", new MeshRendererFactory());
	EntityComponentManager::Instance()->addFactory("FirstPersonCamera", new FirstPersonCameraFactory());
	EntityComponentManager::Instance()->addFactory("DirectionalLight", new DirectionalLightFactory());
	EntityComponentManager::Instance()->addFactory("AudioSource", new AudioSourceFactory());
	EntityComponentManager::Instance()->addFactory("Rigidbody", new RigidBodyFactory());
	EntityComponentManager::Instance()->addFactory("GunController", new GunControllerFactory());
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

		if (sceneCreated_)
		{
			updateScene();
			Physics::Instance()->update();
			EntityComponentManager::Instance()->update();
		}
		else if (UIManager::Instance()->isMenuClosed())
		{
			createScene();
			sceneCreated_ = true;
		}

		RenderManager::Instance()->update(dt);
	}
}

void Application::createScene()
{
	//-----------------READ SCENE-------------------//
	SceneLoader::Instance()->loadScene("Map1.scene");


	//----------------MOUSE OBJECT------------------//
	GameObject* mouse = new GameObject("mouse", "enemy");
	Transform* mouseTransform_ = new Transform(mouse);
	mouseTransform_->setPosition(50.0, 50.0, 0.0);
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

	GameObject* gun = new GameObject("gun", "gun");
	Transform* gunTranform = new Transform(gun, "camera");
	gunTranform->setPosition(Ogre::Vector3(30, -50, -90));
	gunTranform->rotate(Ogre::Vector3(0, 1, 0), 180.0f);
	MeshRenderer* gunRenderer = new MeshRenderer(gun, "gun.mesh");
	gunRenderer->setMaterialName("gun_mat");
	AudioSource* shootSFX = new AudioSource(gun, "ShootSFX", "shoot.wav");

	std::map<std::string, ValueType> params;
	params["enabled"].b = true;
	Component* c = EntityComponentManager::Instance()->getFactory("GunController")->create();
	c->setContainer(gun);
	c->init(params);
	EntityComponentManager::Instance()->addComponent(c);

	//------------------LIGHTS-----------------------//
	// We can create GameObjects and Components on the go,
	// but we can also use the registered factories to do so.
	// First, we create the GameObject as in previous entities:

	GameObject* mainLight = new GameObject("light", "mainLight");

	// Then, we create the data structure that maps the init params.
	// We can reuse this param list for future components, 
	// just make sure you clear it before reusing!

	params.clear();
	params["parent"].s = "";
	params["position_x"].f = 0.0f;
	params["position_y"].f = 20.0f;
	params["position_z"].f = 0.0f;
	params["direction_x"].f = 0.0f;
	params["direction_y"].f = -1.0f;
	params["direction_z"].f = -1.0f;
	params["scale_x"].f = 1.0f;
	params["scale_y"].f = 1.0f;
	params["scale_z"].f = 1.0f;
	params["enabled"].b = true;

	// Now we call the appropriate Factory to create the desired Component:

	Component* c1 = EntityComponentManager::Instance()->getFactory("Transform")->create();

	// We need to attach the component to its container manually.
	// Perhaps we should change the default constructor in Components 
	// to receive a pointer to a GameObject, since all components need one.

	c1->setContainer(mainLight);

	// We init the Component with the data loaded in the params list:

	
	c1->init(params);

	// Same proccess with the next component:

	params.clear();
	params["direction_x"].f = 0.55f;
	params["direction_y"].f = -0.3f;
	params["direction_z"].f = 0.75f;
	params["ambient_r"].f = 0.8f;
	params["ambient_g"].f = 0.8f;
	params["ambient_b"].f = 0.8f;
	params["diffuse_r"].f = 1.0f;
	params["diffuse_g"].f = 1.0f;
	params["diffuse_b"].f = 1.0f;
	params["specular_r"].f = 0.4f;
	params["specular_g"].f = 0.4f;
	params["specular_b"].f = 0.4f;
	params["enabled"].b = true;

	Component* c2 = EntityComponentManager::Instance()->getFactory("DirectionalLight")->create();
	c2->setContainer(mainLight);
	c2->init(params);

	RenderManager::Instance()->getSceneManager()->setSkyDome(true, "skyPlane", 5, 8, 500);
	//Physics::Instance()->setDebugMode(true);
}

void Application::updateScene()
{
	if (InputManager::Instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE))
	{
		if (UIManager::Instance()->isMenuClosed()) UIManager::Instance()->openMenu();
		else RenderManager::Instance()->setRunning(false);
	}
	else if (InputManager::Instance()->getKeyboard()->isKeyDown(OIS::KC_P))
	{
		Physics::Instance()->toggleDebug();
	}
	else if (InputManager::Instance()->getKeyboard()->isKeyDown(OIS::KC_O))
	{
		Physics::Instance()->toggleDebugMode();
	}
	else if (InputManager::Instance()->getKeyboard()->isKeyDown(OIS::KC_F))
	{
		RenderManager::Instance()->getSceneManager()->setFog(Ogre::FOG_EXP2, Ogre::ColourValue::White, 0.001);
	}
}