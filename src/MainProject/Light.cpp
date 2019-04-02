#include "Light.h"
#include "Transform.h"
#include "entitycomponentmanager.h"

std::string Light::name_ = "Light";

Light::Light(GameObject* container, bool enabled) : Component(container, enabled)
{
	initLight();
	static_cast<Transform*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Transform"))->attachEntity(light_);
}

Light::~Light()
{

}

//Initialize the main camera being a child of one node
void Light::initLight()
{
	// without light we would just get a black screen 
	light_ = RenderManager::Instance()->getSceneManager()->createLight("mainLight");
	light_->setType(Ogre::Light::LT_DIRECTIONAL);
	light_->setDiffuseColour(0.75, 0.75, 0.75);
}