#ifndef __DIRECTIONAL_LIGHT_H__
#define __DIRECTIONAL_LIGHT_H__

#include "Component.h"
#include "RenderManager.h"

class DirectionalLight : public Component
{
public:
	DirectionalLight();
	DirectionalLight(GameObject* container, bool enabled);
	virtual ~DirectionalLight();

	virtual void init(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }

private:
	void initLight();
	static std::string name_;

	Ogre::Light* light_;
};

class DirectionalLightFactory : public BaseFactory
{
public:
	DirectionalLightFactory() {}
	virtual ~DirectionalLightFactory() {}

	virtual Component* create() { return new DirectionalLight(); }
};

#endif // !__DIRECTIONAL_LIGHT_H__