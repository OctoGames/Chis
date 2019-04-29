#include "Application.h"

#include "MeshRenderer.h"
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
	Physics::Instance()->init();

	EntityComponentManager::Instance()->registerFactory("MeshRenderer", new MeshRendererFactory());
	EntityComponentManager::Instance()->registerFactory("FirstPersonCamera", new FirstPersonCameraFactory());
	EntityComponentManager::Instance()->registerFactory("DirectionalLight", new DirectionalLightFactory());
	EntityComponentManager::Instance()->registerFactory("AudioSource", new AudioSourceFactory());
	EntityComponentManager::Instance()->registerFactory("RigidBody", new RigidBodyFactory());
	EntityComponentManager::Instance()->registerFactory("GunController", new GunControllerFactory());
}

Application::~Application()
{
	Physics::Instance()->close();
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
			Physics::Instance()->update(dt);
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

	//--------------------SCENE--------------------//

	ArchetypeLoader::Instance()->loadArchetypes();
	SceneLoader::Instance()->loadScene("JaviGuapo.scene");

	RenderManager::Instance()->getSceneManager()->setSkyDome(true, "skyPlane");
	RenderManager::Instance()->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.8f, 0.8f, 0.8f));
	
#if _DEBUG
	Physics::Instance()->setDebugMode(true);
#else
	Physics::Instance()->setDebugMode(false);
#endif


	//-------------------MOUSE---------------------//

	object = new GameObject("MouseStatic", "", "Enemy", true);

	component = EntityComponentManager::Instance()->getFactory("MeshRenderer")->create();
	params["enabled_mr"].b = true;
	params["mesh_name"].s = "mouse.mesh";
	params["material_name"].s = "mouseMaterial";
	component->load(params);
	components.push_back(component);
	params.clear();

	component = EntityComponentManager::Instance()->getFactory("RigidBody")->create();
	params["enabled_rb"].b = true;
	params["mass"].f = 0.0f;
	params["radius"].f = 0.0f;
	params["scale_rb_x"].f = 30.0f;
	params["scale_rb_y"].f = 30.0f;
	params["scale_rb_z"].f = 30.0f;
	component->load(params);
	components.push_back(component);
	params.clear();

	EntityComponentManager::Instance()->registerPrototype(object->getName(), new Prototype(object, components));
	components.clear();

	GameObject* mouseStatic = EntityComponentManager::Instance()->instantiate("MouseStatic", { 0.0f, 50.0f, 0.0f });
	mouseStatic->transform()->setScale(30.0, 30.0, 30.0);

	object = new GameObject("MouseDynamic", "", "Enemy", true);

	component = EntityComponentManager::Instance()->getFactory("MeshRenderer")->create();
	params["enabled_mr"].b = true;
	params["mesh_name"].s = "mouse.mesh";
	params["material_name"].s = "mouseMaterial";
	component->load(params);
	components.push_back(component);
	params.clear();

	component = EntityComponentManager::Instance()->getFactory("RigidBody")->create();
	params["enabled_rb"].b = true;
	params["mass"].f = 10.0f;
	params["radius"].f = 0.0f;
	params["scale_rb_x"].f = 30.0f;
	params["scale_rb_y"].f = 30.0f;
	params["scale_rb_z"].f = 30.0f;
	component->load(params);
	components.push_back(component);
	params.clear();

	EntityComponentManager::Instance()->registerPrototype(object->getName(), new Prototype(object, components));
	components.clear();

	GameObject* mouseDynamic = EntityComponentManager::Instance()->instantiate("MouseDynamic", { 26.0f, 150.0f, 0.0f });
	mouseDynamic->transform()->setScale(30.0, 30.0, 30.0);

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

	EntityComponentManager::Instance()->registerPrototype("DirLight", new Prototype(object, components));
	components.clear();

	GameObject* light = EntityComponentManager::Instance()->instantiate("DirLight", { 0.0, 20.0, 0.0 });


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
	params["forward_key"].f = OIS::KC_W;
	params["backward_key"].f = OIS::KC_S;
	params["fast_key"].f = OIS::KC_LSHIFT;
	params["left_key"].f = OIS::KC_A;
	params["right_key"].f = OIS::KC_D;
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

	EntityComponentManager::Instance()->registerPrototype("Player", new Prototype(object, components));
	components.clear();

	GameObject* player = EntityComponentManager::Instance()->instantiate("Player", { 200.0, 100.0, 400.0 });
	static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(player, "AudioSource"))->play();



	//----------------------GUN----------------------//

	object = new GameObject("Gun", "Player", "Gun", true);

	component = EntityComponentManager::Instance()->getFactory("MeshRenderer")->create();
	params["enabled_mr"].b = true;
	params["mesh_name"].s = "Brazos.mesh";
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

	EntityComponentManager::Instance()->registerPrototype("Gun", new Prototype(object, components));
	components.clear();

	GameObject* gun = EntityComponentManager::Instance()->instantiate("Gun", { 1.0, -2.5, -4.0 }, Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90)), Ogre::Vector3::UNIT_Y));

	MeshRenderer* mr = static_cast<MeshRenderer*>(EntityComponentManager::Instance()->getComponent(gun, "MeshRenderer"));
	Ogre::Entity* ent = mr->getEntity();

	std::cout << "Bones names: " << std::endl;
	auto skeleton = ent->getMesh()->getSkeleton();
	auto numBones = skeleton->getNumBones();
	for (int i = 0; i < numBones; i++) std::cout << skeleton->getBone(i)->getName() << std::endl;


	Ogre::Entity* leftSword = RenderManager::Instance()->getSceneManager()->createEntity("PiezaArma1.mesh");
	ent->attachObjectToBone("Bone.003", leftSword);

	std::cout << "Animations names: " << std::endl;
	Ogre::AnimationStateSet* aux = ent->getAllAnimationStates();
	auto it = aux->getAnimationStateIterator().begin();
	while (it != aux->getAnimationStateIterator().end())
	{
		auto s = it->first; ++it;
		std::cout << s << std::endl;
	}

	Ogre::AnimationState* animationState = ent->getAnimationState("my_animation");
	animationState->setEnabled(true);
	animationState->setLoop(true);
}