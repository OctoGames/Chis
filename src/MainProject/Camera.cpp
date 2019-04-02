#include "Camera.h"
#include "Transform.h"
#include "entitycomponentmanager.h"

std::string Camera::name_ = "Camera";

Camera::Camera(GameObject* container, bool enabled) : Component(container, enabled)
{
	initCamera();
	static_cast<Transform*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Transform"))->attachEntity(camera_);
}

Camera::~Camera()
{

}

//Initialize the main camera being a child of one node
void Camera::initCamera()
{
	//Camera
	camera_ = RenderManager::Instance()->getSceneManager()->createCamera("mainCamera");
	camera_->setNearClipDistance(1);
	camera_->setFarClipDistance(10000);
	camera_->setAutoAspectRatio(true);

	//Viewport
	viewport_ = RenderManager::Instance()->getWindow()->addViewport(camera_);
	viewport_->setBackgroundColour(Ogre::ColourValue(0.0, 0.5, 0.5));
}