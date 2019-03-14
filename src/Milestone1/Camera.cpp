#include "Camera.h"
#include "Transform.h"
#include "EntityComponentManager.h"

std::string Camera::name_ = "Camera";

Camera::Camera(GameObject* container, bool enabled) : 
	Component(container, enabled)
{
	//Camera
	camera_ = OgreSystem::Instance()->getSceneManager()->createCamera("mainCamera");
	camera_->setNearClipDistance(1);
	camera_->setFarClipDistance(10000);
	camera_->setAutoAspectRatio(true);

	//Viewport
	viewport_ = OgreSystem::Instance()->getWindow()->addViewport(camera_);
	viewport_->setBackgroundColour(Ogre::ColourValue(0.7, 0.6, 0.0));

	static_cast<Transform*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Transform"))->attachEntity(camera_);
}

Camera::~Camera()
{
}