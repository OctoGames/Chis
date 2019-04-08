#include "Application.h"

#include "Transform.h"
#include "MeshRenderer.h"
#include "FirstPersonCamera.h"
#include "DirectionalLight.h"
#include "AudioSource.h"
#include "RigidBody.h"

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
	//GraphGenerator* graphG = new GraphGenerator();
	//graphG->initG();
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

	//------------------LIGHTS-----------------------//

	// We can create GameObjects and Components on the go,
	// but we can also use the registered factories to do so.
	// First, we create the GameObject as in previous entities:

	GameObject* mainLight = new GameObject("light", "mainLight");

	// Then, we create the data structure that maps the init params.
	// We can reuse this param list for future components, 
	// just make sure you clear it before reusing!

	std::map<std::string, ValueType> params;
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
	params["diffuse_r"].f = 0.75f;
	params["diffuse_g"].f = 0.75f;
	params["diffuse_b"].f = 0.75f;
	params["enabled"].b = true;

	Component* c2 = EntityComponentManager::Instance()->getFactory("DirectionalLight")->create();
	c2->setContainer(mainLight);
	c2->init(params);

	Physics::Instance()->setDebugMode(true);
}

void Application::updateScene()
{
	if (InputManager::Instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE))
	{
		if (UIManager::Instance()->isMenuClosed()) UIManager::Instance()->openMenu();
		//RenderManager::Instance()->setRunning(false);
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