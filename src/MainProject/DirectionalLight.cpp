#include "DirectionalLight.h"

#include "EntityComponentManager.h"

std::string DirectionalLight::name_ = "Light";

DirectionalLight::DirectionalLight() :
	light_(nullptr),
	lightDirection_(Ogre::Vector3::ZERO),
	diffuseColour_(Ogre::ColourValue::ZERO),
	specularColour_(Ogre::ColourValue::ZERO)
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::load(const std::map<std::string, ValueType>& params)
{
	enabled_ = params.at("enabled_dl").b;
	lightDirection_ = Ogre::Vector3(params.at("direction_x").f, params.at("direction_y").f, params.at("direction_z").f); 
	diffuseColour_ = Ogre::ColourValue(params.at("diffuse_r").f, params.at("diffuse_g").f, params.at("diffuse_b").f);
	specularColour_ = Ogre::ColourValue(params.at("specular_r").f, params.at("specular_g").f, params.at("specular_b").f);
}

Component * DirectionalLight::clone()
{
	DirectionalLight* clonedComponent = new DirectionalLight();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->lightDirection_ = this->lightDirection_;
	clonedComponent->diffuseColour_ = this->diffuseColour_;
	clonedComponent->specularColour_ = this->specularColour_;

	return clonedComponent;
}

void DirectionalLight::init()
{
	light_ = RenderManager::Instance()->getSceneManager()->createLight("DirectionalLight_" + gameObject()->getName());
	light_->setType(Ogre::Light::LT_DIRECTIONAL);
	light_->setDirection(lightDirection_);
	lightDirection_.normalise();
	light_->setDiffuseColour(diffuseColour_);
	light_->setSpecularColour(specularColour_);

	gameObject()->transform()->attachObject(light_);
	setEnabled(enabled_);
}