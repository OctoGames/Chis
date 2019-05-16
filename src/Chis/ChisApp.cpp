#include "ChisApp.h"

#include "MeshRenderer.h"
#include "FirstPersonCamera.h"
#include "FirstPersonMovement.h"
#include "DirectionalLight.h"
#include "AudioSource.h"
#include "RigidBody.h"
#include "GunController.h"
#include "GameManager.h"
#include "Camera.h"
#include "Canvas.h"
#include "SceneManager.h"
#include "PlayerController.h"
#include "Player.h"
#include "Ammo.h"
#include "Enemy.h"
#include "Cheese.h"
#include "RaycastBullet.h"
#include "RigidbodyBullet.h"
#include "Node.h"

bool ChisApp::reset_ = false;

ChisApp::ChisApp()
{
}

ChisApp::~ChisApp()
{
}

void ChisApp::init()
{
	Application::init();
	AIManager::Instance()->init("Assets/AIFiles/graphMap01.txt");

	registerFactories();

	ArchetypeLoader::Instance()->loadArchetypes();
	EntityComponentManager::Instance()->instantiate("GameManager");
}

void ChisApp::run()
{
	Application::run();
	if (reset_)
	{
		reset_ = false;
		reset();
	}
}

void ChisApp::close()
{
	Application::close();
	AIManager::Instance()->close();
}

void ChisApp::registerFactories()
{
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
	EntityComponentManager::Instance()->registerFactory("FirstPersonMovement", new FirstPersonMovementFactory());
	EntityComponentManager::Instance()->registerFactory("PlayerController", new PlayerControllerFactory());
	EntityComponentManager::Instance()->registerFactory("Player", new PlayerFactory());
	EntityComponentManager::Instance()->registerFactory("Ammo", new AmmoFactory());
	EntityComponentManager::Instance()->registerFactory("Enemy", new EnemyFactory());
	EntityComponentManager::Instance()->registerFactory("Cheese", new CheeseFactory());
	EntityComponentManager::Instance()->registerFactory("RaycastBullet", new RaycastBulletFactory());
	EntityComponentManager::Instance()->registerFactory("RigidbodyBullet", new RigidbodyBulletFactory());
	EntityComponentManager::Instance()->registerFactory("Node", new NodeFactory());
}

void ChisApp::reset()
{
	close();
	init();
	run();
}