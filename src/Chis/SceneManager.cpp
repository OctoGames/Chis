#include "SceneManager.h"

#include "Camera.h"
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
	return;
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

void SceneManager::createMenuScene()
{
	RenderManager::Instance()->getSceneManager()->setSkyPlane(true, Ogre::Plane(Ogre::Vector3::UNIT_Z, -50), "Cheese", 1, 1, true, 1.0, 100, 100);
	RenderManager::Instance()->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.8f, 0.8f, 0.8f));
}

void SceneManager::createGameScene()
{
	SceneLoader::Instance()->loadScene("Scene1.scene");
	RenderManager::Instance()->getSceneManager()->setSkyPlaneEnabled(false);
	RenderManager::Instance()->getSceneManager()->setSkyDome(true, "skyPlane");
	RenderManager::Instance()->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.8f, 0.8f, 0.8f));

	AudioSource* song = static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"));
	song->play();
}