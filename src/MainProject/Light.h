#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Component.h"
#include "RenderManager.h"

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

#endif // !__LIGHT_H__