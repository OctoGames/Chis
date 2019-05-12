#include "SceneManager.h"

#include "Camera.h"
#include "Canvas.h"
#include "AudioSource.h"


std::string SceneManager::name_ = "SceneManager";

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_sm"); if (it != params.end()) enabled_ = params.at("enabled_sm").b;
}

Component * SceneManager::clone()
{
	SceneManager* clonedComponent = new SceneManager();

	clonedComponent->enabled_ = this->enabled_;

	return clonedComponent;
}

void SceneManager::init()
{
	setEnabled(enabled_);
}

void SceneManager::start()
{
	Canvas* canvas = static_cast<Canvas*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Canvas"));
	canvas->toMainMenu();
}

void SceneManager::createMenuScene()
{
	clearScene();
	RenderManager::Instance()->getSceneManager()->setSkyPlane(true, Ogre::Plane(Ogre::Vector3::UNIT_Z, -50), "Cheese", 1, 1, true, 1.0, 100, 100);

	Camera* cam = static_cast<Camera*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Camera"));
	cam->getCamera()->lookAt({ 0, 0, 0 });

	AudioSource* song = static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"));
	song->changeSource("MusicaFondo.wav");
	song->play();
}

void SceneManager::createGameScene()
{
	clearScene();
	RenderManager::Instance()->getSceneManager()->setSkyDome(true, "skyPlane");

	SceneLoader::Instance()->loadScene("Scene1.scene");

	AudioSource* song = static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"));
	song->stop();
	song->changeSource("22-The Mouse's House.mp3");
	song->play();
}

void SceneManager::createEndScene()
{
	clearScene();
	RenderManager::Instance()->getSceneManager()->setSkyPlane(true, Ogre::Plane(Ogre::Vector3::UNIT_Z, -50), "Cheese", 1, 1, true, 1.0, 100, 100);

	Camera* cam = static_cast<Camera*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Camera"));
	cam->getCamera()->lookAt({ 0, 0, 0 });

	AudioSource* song = static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"));
	song->stop();
	song->changeSource("MusicaFondo.wav");
	song->play();
}

void SceneManager::clearScene()
{
	EntityComponentManager::Instance()->destroyAll();
	RenderManager::Instance()->getSceneManager()->clearScene();
	RenderManager::Instance()->getSceneManager()->destroyCamera("FirstPersonCamera");
	RenderManager::Instance()->getSceneManager()->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
}
