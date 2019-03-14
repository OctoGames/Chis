#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Component.h"
#include "OgreSystem.h"

class Camera : public Component
{
public:
	Camera(GameObject* container, bool enabled);
	virtual ~Camera();

	virtual std::string getName() const { return name_; }

private:
	static std::string name_;

	Ogre::Camera* camera_;
	Ogre::Viewport* viewport_;
};

#endif // !_CAMERA_H_