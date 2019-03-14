#include "Light.h"
#include "Transform.h"
#include "entitycomponentmanager.h"

std::string Light::name_ = "Light";

Light::Light(GameObject* container, bool enabled) : 
	Component(container, enabled)
{
	light_ = OgreSystem::Instance()->getSceneManager()->createLight("mainLight");
	light_->setType(Ogre::Light::LT_DIRECTIONAL);
	light_->setDiffuseColour(0.75, 0.75, 0.75);
	static_cast<Transform*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Transform"))->attachEntity(light_);
}

Light::~Light()
{
}