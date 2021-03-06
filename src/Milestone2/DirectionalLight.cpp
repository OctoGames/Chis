#include "DirectionalLight.h"
#include "Transform.h"
#include "entitycomponentmanager.h"

std::string DirectionalLight::name_ = "Light";

DirectionalLight::DirectionalLight() :
	light_(nullptr)
{
}

DirectionalLight::DirectionalLight(GameObject* container, bool enabled) : 
	Component(container, enabled)
{
	initLight();
	static_cast<Transform*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Transform"))->attachEntity(light_);
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::init(const std::map<std::string, ValueType>& params)
{
	light_ = RenderManager::Instance()->getSceneManager()->createLight("mainLight");
	light_->setType(Ogre::Light::LT_DIRECTIONAL);

	Ogre::Vector3 lightdir(params.at("direction_x").f, params.at("direction_y").f, params.at("direction_z").f); lightdir.normalise();
	light_->setDirection(lightdir);

	RenderManager::Instance()->getSceneManager()->setAmbientLight(Ogre::ColourValue(params.at("ambient_r").f, params.at("ambient_g").f, params.at("ambient_b").f));
	light_->setDiffuseColour(params.at("diffuse_r").f, params.at("diffuse_g").f, params.at("diffuse_b").f);
	light_->setSpecularColour(params.at("specular_r").f, params.at("specular_g").f, params.at("specular_b").f);

	static_cast<Transform*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Transform"))->attachEntity(light_);

	setEnabled(params.at("enabled").b);
}

void DirectionalLight::initLight()
{
	light_ = RenderManager::Instance()->getSceneManager()->createLight("mainLight");
	light_->setType(Ogre::Light::LT_DIRECTIONAL);
	light_->setDiffuseColour(0.75, 0.75, 0.75);
}