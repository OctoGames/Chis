#include "Application.h"

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

	EntityComponentManager::Instance()->addFactory("MeshRenderer", new MeshRendererFactory());
	EntityComponentManager::Instance()->addFactory("FirstPersonCamera", new FirstPersonCameraFactory());
	EntityComponentManager::Instance()->addFactory("DirectionalLight", new DirectionalLightFactory());
	EntityComponentManager::Instance()->addFactory("AudioSource", new AudioSourceFactory());
	EntityComponentManager::Instance()->addFactory("RigidBody", new RigidBodyFactory());
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

		if (sceneCreated_ && UIManager::Instance()->isMenuClosed())
		{
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
	GameObject* object = nullptr;
	Component* component = nullptr;
	std::list<Component*> components;
	std::map<std::string, ValueType> params;


	//-------------------MOUSE---------------------//
	
	object = new GameObject("Mouse", "", "Enemy", true);
	
	component = EntityComponentManager::Instance()->getFactory("MeshRenderer")->create();
	params["enabled_mr"].b = true;
	params["mesh_name"].s = "mouse.mesh";
	params["material_name"].s = "mouseMaterial";
	component->load(params);
	components.push_back(component);
	params.clear();

	component = EntityComponentManager::Instance()->getFactory("RigidBody")->create();
	params["enabled_rb"].b = true;
	params["mass"].d = 0.0;
	params["radius"].d = 0.0;
	params["scale_x"].d = 50.0;
	params["scale_y"].d = 30.0;
	params["scale_z"].d = 30.0;
	component->load(params);
	components.push_back(component);
	params.clear();

	EntityComponentManager::Instance()->addPrototype(new Prototype("MouseEnemy", object, components));	
	components.clear();

	GameObject* mouse = EntityComponentManager::Instance()->instantiate("MouseEnemy");
	mouse->transform()->setPosition(50.0, 60.0, 0.0);
	mouse->transform()->setScale(30.0, 30.0, 30.0);



	//-------------------LIGHT-----------------------//

	object = new GameObject("MainLight", "", "Light", true);

	component = EntityComponentManager::Instance()->getFactory("DirectionalLight")->create();
	params["enabled"].b = true;
	params["direction_x"].f = 0.55f;
	params["direction_y"].f = -0.3f;
	params["direction_z"].f = 0.75f;
	params["diffuse_r"].f = 1.0f;
	params["diffuse_g"].f = 1.0f;
	params["diffuse_b"].f = 1.0f;
	params["specular_r"].f = 0.4f;
	params["specular_g"].f = 0.4f;
	params["specular_b"].f = 0.4f;
	component->load(params);
	components.push_back(component);
	params.clear();

	EntityComponentManager::Instance()->addPrototype(new Prototype("DirLight", object, components));
	components.clear();

	GameObject* light = EntityComponentManager::Instance()->instantiate("DirLight");
	light->transform()->setPosition(0.0, 20.0, 0.0);



	//--------------------PLAYER--------------------//

	object = new GameObject("Player", "", "Player", true);

	component = EntityComponentManager::Instance()->getFactory("FirstPersonCamera")->create();
	params["enabled"].b = true;
	params["far_clip"].f = 10000.0f;
	params["near_clip"].f = 1.5f;
	params["color_r"].f = 1.0f;
	params["color_g"].f = 0.0f;
	params["color_b"].f = 1.0f;
	params["max_speed"].f = 200.0f;
	params["pitch_limit"].f = 180.0f;
	params["forward_key"].i = OIS::KC_W;
	params["backward_key"].i = OIS::KC_S;
	params["fast_key"].i = OIS::KC_LSHIFT;
	params["left_key"].i = OIS::KC_A;
	params["right_key"].i = OIS::KC_D;
	component->load(params);
	components.push_back(component);
	params.clear();

	component = EntityComponentManager::Instance()->getFactory("AudioSource")->create();
	params["enabled"].b = true;
	params["filename"].s = "22-The Mouse's House.mp3";
	params["audio_id"].s = "MouseMusic";
	params["volume"].f = 1.0f;
	params["pitch"].f = 1.0f;
	component->load(params);
	components.push_back(component);
	params.clear();

	EntityComponentManager::Instance()->addPrototype(new Prototype("Player", object, components));
	components.clear();

	GameObject* player = EntityComponentManager::Instance()->instantiate("Player");
	player->transform()->setPosition(200.0, 100.0, 400.0);
	player->transform()->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
	static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(player, "AudioSource"))->play();



	//----------------------GUN----------------------//

	object = new GameObject("Gun", "Player", "Gun", true);

	component = EntityComponentManager::Instance()->getFactory("MeshRenderer")->create();
	params["enabled_mr"].b = true;
	params["mesh_name"].s = "gun.mesh";
	params["material_name"].s = "gunMaterial";
	component->load(params);
	components.push_back(component);
	params.clear();

	component = EntityComponentManager::Instance()->getFactory("AudioSource")->create();
	params["enabled"].b = true;
	params["filename"].s = "shoot.wav";
	params["audio_id"].s = "ShootSFX";
	params["volume"].f = 1.0f;
	params["pitch"].f = 1.0f;
	component->load(params);
	components.push_back(component);
	params.clear();

	component = EntityComponentManager::Instance()->getFactory("GunController")->create();
	params["enabled"].b = true;
	params["fire_button"].i = OIS::MouseButtonID::MB_Left;
	component->load(params);
	components.push_back(component);
	params.clear();

	EntityComponentManager::Instance()->addPrototype(new Prototype("Gun", object, components));
	components.clear();

	GameObject* gun = EntityComponentManager::Instance()->instantiate("Gun");
	gun->transform()->setPosition(Ogre::Vector3(30, -50, -90));
	gun->transform()->rotate(Ogre::Vector3(0, 1, 0), Ogre::Radian(Ogre::Degree(180.0f)));



	//--------------------SCENE--------------------//

	ArchetypeLoader::Instance()->loadArchetypes();

	//params["enabled_mr"].b = true;
	//params["enabled_rb"].b = true;
	//params["mesh_name"].s = "Donut.mesh";
	//params["material_name"].s = "donut";
	//params["mass"].f = 0.0f;
	//params["radius"].f = 0.0f;
	//params["scale_x"].f = 3.0f;
	//params["scale_y"].f = 3.0f;
	//params["scale_z"].f = 3.0f;

	//Prototype* prototype = EntityComponentManager::Instance()->getPrototype("obstaculo");
	//GameObject* clonedObject = prototype->getEntity()->clone();
	//clonedObject->setName("obstaculo");
	//clonedObject->transform()->setPosition(Ogre::Vector3(50.0, 50.0, 0.0));
	//clonedObject->transform()->setScale(Ogre::Vector3(3.0,3.0,3.0));

	//for (Component* c : prototype->getComponents()) {
	//	Component* clonedComponent = c->clone();
	//	clonedComponent->load(params);
	//	clonedComponent->setContainer(clonedObject);
	//	clonedComponent->init();
	//}

	//GameObject* obs = EntityComponentManager::Instance()->instantiate("obstaculo");
	//obs->transform()->setPosition(Ogre::Vector3(50, 50, 0));
	//obs->transform()->setScale(Ogre::Vector3(3.0, 3.0, 3.0));
	
	
	
	
	
	
	SceneLoader::Instance()->loadScene("JaviGuapo.scene");
	RenderManager::Instance()->getSceneManager()->setSkyDome(true, "skyPlane");
	RenderManager::Instance()->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.8f, 0.8f, 0.8f));
	Physics::Instance()->setDebugMode(true);
}