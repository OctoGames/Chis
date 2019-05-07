#ifndef __DIRECTIONAL_LIGHT_H__
#define __DIRECTIONAL_LIGHT_H__

#include "Engine.h"

class DirectionalLight : public Component
{
public:
	DirectionalLight();
	virtual ~DirectionalLight();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

private:
	static std::string name_;

	Ogre::Light* light_;
	Ogre::Vector3 lightDirection_;
	Ogre::ColourValue diffuseColour_;
	Ogre::ColourValue specularColour_;
};

class DirectionalLightFactory : public BaseFactory
{
public:
	DirectionalLightFactory() {}
	virtual ~DirectionalLightFactory() {}

	virtual Component* create() { return new DirectionalLight(); }
};

#endif // !__DIRECTIONAL_LIGHT_H__