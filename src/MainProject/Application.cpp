#include "Application.h"

#include "MeshRenderer.h"
#include "FirstPersonCamera.h"
#include "DirectionalLight.h"
#include "AudioSource.h"
#include "RigidBody.h"
#include "GunController.h"
#include "GameManager.h"
#include "Camera.h"
#include "Canvas.h"
#include "SceneManager.h"

Application::Application()
{
	// Entry Point (Engine Application)
	RenderManager::Instance()->init();
	InputManager::Instance()->init();
	GUIManager::Instance()->init();
	Physics::Instance()->init();

	// Init function (Game Application)
	EntityComponentManager::Instance()->registerFactory("MeshRenderer", new MeshRendererFactory());
	EntityComponentManager::Instance()->registerFactory("FirstPersonCamera", new FirstPersonCameraFactory());
	EntityComponentManager::Instance()->registerFactory("DirectionalLight", new DirectionalLightFactory());
	EntityComponentManager::Instance()->registerFactory("AudioSource", new AudioSourceFactory());
	EntityComponentManager::Instance()->registerFactory("RigidBody", new RigidBodyFactory());
	EntityComponentManager::Instance()->registerFactory("GunController", new GunControllerFactory());
	EntityComponentManager::Instance()->registerFactory("GameManager", new GameManagerFactory());
	EntityComponentManager::Instance()->registerFactory("Camera", new CameraFactory());
	EntityComponentManager::Instance()->registerFactory("Canvas", new CanvasFactory());
	EntityComponentManager::Instance()->registerFactory("SceneManager", new SceneManagerFactory());
	
	ArchetypeLoader::Instance()->loadArchetypes();
	EntityComponentManager::Instance()->instantiate("GameManager", { 200.0, 100.0, 400.0 });
}

Application::~Application()
{
	// Shutdown function (Engine Application)
	Physics::Instance()->close();
	GUIManager::Instance()->close();
	InputManager::Instance()->close();
	RenderManager::Instance()->close();
}

void Application::run()
{
	// Virtual run function (Engine Application)
	while (RenderManager::Instance()->isRunning())
	{
		float dt = RenderManager::Instance()->time()->tick();
		if (dt == 0.0f) continue;

		AudioSystem::Instance()->update();
		InputManager::Instance()->update(dt);
		Physics::Instance()->update(dt);
		EntityComponentManager::Instance()->update();
		GUIManager::Instance()->update(dt);
		RenderManager::Instance()->update(dt);
	}
}

//void Application::createScene()
//{
//	//--------------------SCENE--------------------//
//	SceneLoader::Instance()->loadScene("Scene1.scene");
//	RenderManager::Instance()->getSceneManager()->setSkyDome(true, "skyPlane");
//	RenderManager::Instance()->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.8f, 0.8f, 0.8f));
//
//#if _DEBUG
//	// If true the framerate drops too much
//	Physics::Instance()->setDebugMode(false);
//#else
//	Physics::Instance()->setDebugMode(false);
//#endif
//	
//	//-------------------ARCHETYPES---------------------//	
//	//object = new GameObject("Bullet", "", "Bullet", true);
//	//component = EntityComponentManager::Instance()->getFactory("MeshRenderer")->create();
//	//params["enabled_mr"].b = true;
//	//params["mesh_name"].s = "Bala2.mesh";
//	//params["material_name"].s = "";
//	//component->load(params);
//	//components.push_back(component);
//	//params.clear();
//	//component = EntityComponentManager::Instance()->getFactory("RigidBody")->create();
//	//params["enabled_rb"].b = true;
//	//params["mass"].f = 1.0f;
//	//params["radius"].f = 1.0f;
//	//params["scale_rb_x"].f = 0.0f;
//	//params["scale_rb_y"].f = 0.0f;
//	//params["scale_rb_z"].f = 0.0f;
//	//component->load(params);
//	//components.push_back(component);
//	//params.clear();
//	//EntityComponentManager::Instance()->registerPrototype(object->getName(), new Prototype(object, components));
//	//components.clear();
//	//object = new GameObject("MouseStatic", "", "Enemy", true);
//	//component = EntityComponentManager::Instance()->getFactory("MeshRenderer")->create();
//	//params["enabled_mr"].b = true;
//	//params["mesh_name"].s = "mouse.mesh";
//	//params["material_name"].s = "mouseMaterial";
//	//component->load(params);
//	//components.push_back(component);
//	//params.clear();
//	//component = EntityComponentManager::Instance()->getFactory("RigidBody")->create();
//	//params["enabled_rb"].b = true;
//	//params["mass"].f = 0.0f;
//	//params["radius"].f = 0.0f;
//	//params["scale_rb_x"].f = 30.0f;
//	//params["scale_rb_y"].f = 30.0f;
//	//params["scale_rb_z"].f = 30.0f;
//	//component->load(params);
//	//components.push_back(component);
//	//params.clear();
//	//EntityComponentManager::Instance()->registerPrototype(object->getName(), new Prototype(object, components));
//	//components.clear();
//	//object = new GameObject("MouseDynamic", "", "Enemy", true);
//	//component = EntityComponentManager::Instance()->getFactory("MeshRenderer")->create();
//	//params["enabled_mr"].b = true;
//	//params["mesh_name"].s = "mouse.mesh";
//	//params["material_name"].s = "mouseMaterial";
//	//component->load(params);
//	//components.push_back(component);
//	//params.clear();
//	//component = EntityComponentManager::Instance()->getFactory("RigidBody")->create();
//	//params["enabled_rb"].b = true;
//	//params["mass"].f = 10.0f;
//	//params["radius"].f = 0.0f;
//	//params["scale_rb_x"].f = 30.0f;
//	//params["scale_rb_y"].f = 30.0f;
//	//params["scale_rb_z"].f = 30.0f;
//	//component->load(params);
//	//components.push_back(component);
//	//params.clear();
//	//EntityComponentManager::Instance()->registerPrototype(object->getName(), new Prototype(object, components));
//	//components.clear();
//	//object = new GameObject("MainLight", "", "Light", true);
//	//component = EntityComponentManager::Instance()->getFactory("DirectionalLight")->create();
//	//params["enabled_dl"].b = true;
//	//params["direction_x"].f = 0.55f;
//	//params["direction_y"].f = -0.3f;
//	//params["direction_z"].f = 0.75f;
//	//params["diffuse_r"].f = 1.0f;
//	//params["diffuse_g"].f = 1.0f;
//	//params["diffuse_b"].f = 1.0f;
//	//params["specular_r"].f = 0.4f;
//	//params["specular_g"].f = 0.4f;
//	//params["specular_b"].f = 0.4f;
//	//component->load(params);
//	//components.push_back(component);
//	//params.clear();
//	//EntityComponentManager::Instance()->registerPrototype("DirLight", new Prototype(object, components));
//	//components.clear();
//	//object = new GameObject("Player", "", "Player", true);
//	//component = EntityComponentManager::Instance()->getFactory("FirstPersonCamera")->create();
//	//params["enabled_fpc"].b = true;
//	//params["far_clip"].f = 10000.0f;
//	//params["near_clip"].f = 0.5f;
//	//params["color_r"].f = 1.0f;
//	//params["color_g"].f = 0.0f;
//	//params["color_b"].f = 1.0f;
//	//params["max_speed"].f = 200.0f;
//	//params["pitch_limit"].f = 180.0f;
//	//params["forward_key"].i = OIS::KC_W;
//	//params["backward_key"].i = OIS::KC_S;
//	//params["fast_key"].i = OIS::KC_LSHIFT;
//	//params["left_key"].i = OIS::KC_A;
//	//params["right_key"].i = OIS::KC_D;
//	//component->load(params);
//	//components.push_back(component);
//	//params.clear();
//
//	//EntityComponentManager::Instance()->registerPrototype("Player", new Prototype(object, components));
//	//components.clear();
//	//static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(player, "AudioSource"))->play();
//	//object = new GameObject("Arms", "Player", "Gun", true);
//	//component = EntityComponentManager::Instance()->getFactory("MeshRenderer")->create();
//	//params["enabled_mr"].b = true;
//	//params["mesh_name"].s = "arms.mesh";
//	//component->load(params);
//	//components.push_back(component);
//	//params.clear();
//	//component = EntityComponentManager::Instance()->getFactory("AudioSource")->create();
//	//params["enabled_as"].b = true;
//	//params["filename"].s = "shoot.wav";
//	//params["audio_id"].s = "ShootSFX";
//	//params["volume"].f = 1.0f;
//	//params["pitch"].f = 1.0f;
//	//component->load(params);
//	//components.push_back(component);
//	//params.clear();
//	//component = EntityComponentManager::Instance()->getFactory("GunController")->create();
//	//params["enabled_gc"].b = true;
//	//params["fire_button"].i = OIS::MouseButtonID::MB_Left;
//	//component->load(params);
//	//components.push_back(component);
//	//params.clear();
//	//EntityComponentManager::Instance()->registerPrototype(object->getName(), new Prototype(object, components));
//	//components.clear();
//
//	//MeshRenderer* mr = static_cast<MeshRenderer*>(EntityComponentManager::Instance()->getComponent(gun, "MeshRenderer"));
//	//Ogre::Entity* ent = mr->getEntity();
//	//std::cout << "Bones names: " << std::endl;
//	//auto skeleton = ent->getMesh()->getSkeleton();
//	//auto numBones = skeleton->getNumBones();
//	//for (int i = 0; i < numBones; i++) std::cout << skeleton->getBone(i)->getName() << std::endl;
//	//Ogre::Entity* leftSword = RenderManager::Instance()->getSceneManager()->createEntity("PiezaArma1.mesh");
//	//ent->attachObjectToBone("Bone.003", leftSword);
//	//std::cout << "Animations names: " << std::endl;
//	//Ogre::AnimationStateSet* aux = ent->getAllAnimationStates();
//	//auto it = aux->getAnimationStateIterator().begin();
//	//while (it != aux->getAnimationStateIterator().end())
//	//{
//	//	auto s = it->first; ++it;
//	//	std::cout << s << std::endl;
//	//}
//	//Ogre::AnimationState* animationState = ent->getAnimationState("my_animation");
//	//animationState->setEnabled(true);
//	//animationState->setLoop(true);
//
//	//IA //GraphGenerator::Instance()->init();
//	//Prototype* prototype = EntityComponentManager::Instance()->getPrototype("obstaculo");
//	//GameObject* clonedObject = prototype->getEntity()->clone();
//	//clonedObject->setName("obstaculo");
//	//clonedObject->transform()->setPosition(Ogre::Vector3(50.0, 50.0, 0.0));
//	//clonedObject->transform()->setScale(Ogre::Vector3(10.0023, 10.0023, 10.0023));
//	//for (Component* c : prototype->getComponents()) {
//	//	Component* clonedComponent = c->clone();
//	//	clonedComponent->load(params);
//	//	clonedComponent->setContainer(clonedObject);
//	//	clonedComponent->init();
//	//}
//	//GameObject* obs = EntityComponentManager::Instance()->instantiate("obstaculo");
//	//obs->transform()->setPosition(Ogre::Vector3(50, 50, 0));
//	//obs->transform()->setScale(Ogre::Vector3(3.0, 3.0, 3.0));
//
//	//-------------------CLONES---------------------//
//	//GameObject* light = EntityComponentManager::Instance()->instantiate("dirlight", { 0.0, 20.0, 0.0 });
//	//GameObject* mouse1 = EntityComponentManager::Instance()->instantiate("mouse", { 0.0f, 50.0f, 0.0f });
//	//mouse1->transform()->setScale(30.0, 30.0, 30.0);
//	//GameObject* mouse2 = EntityComponentManager::Instance()->instantiate("mouse", { 26.0f, 150.0f, 0.0f });
//	//mouse2->transform()->setScale(30.0, 30.0, 30.0);
//	//GameObject* player = EntityComponentManager::Instance()->instantiate("player", { 200.0, 100.0, 400.0 });
//	//GameObject* gun = EntityComponentManager::Instance()->instantiate("arms", { -3.0, -4.0, 0.0 }, Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90)), Ogre::Vector3::UNIT_Y));
//	//gun->transform()->setScale(0.1, 0.1, 0.1);
//}