#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Component.h"
#include "OgreSystem.h"

class Light : public Component
{
public:
	Light(GameObject* container, bool enabled);
	virtual ~Light();

	virtual std::string getName() const { return name_; }

private:
	static std::string name_;

	Ogre::Light* light_;
};

#endif // !_LIGHT_H_
