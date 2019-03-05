#pragma once
#include "Application.h"
#include "component.h"

class Light : public Component
{
public:
	Light(GameObject* container, bool enabled);
	virtual ~Light();

	virtual std::string getName() const { return name_; }

private:

	void initLight();
	static std::string name_;

	Ogre::Light* light_;
};