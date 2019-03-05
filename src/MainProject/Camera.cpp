#include "Camera.h"
#include "Transform.h"
#include "entitycomponentmanager.h"

std::string Camera::name_ = "Camera";

Camera::Camera(GameObject* container, bool enabled) : Component(container, enabled)
{
	initCamera();
	static_cast<Transform*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Transform"))->attachEntity(mainCamera);
}

Camera::~Camera()
{

}

//Initialize the main camera being a child of one node
void Camera::initCamera()
{
	//Camera
	mainCamera = Application::Instance()->getSceneManager()->createCamera("mainCamera");
	mainCamera->setNearClipDistance(1);
	mainCamera->setFarClipDistance(10000);
	mainCamera->setAutoAspectRatio(true);

	//Viewport
	viewport = Application::Instance()->getWindow()->addViewport(mainCamera);
	viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.5, 0.5));
}