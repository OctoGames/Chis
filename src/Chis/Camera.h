#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Engine.h"

class Camera : public Component
{
public:
	Camera();
	~Camera();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

private:
	static std::string name_;

	Ogre::Camera* camera_;
	Ogre::Viewport* viewport_;

	Ogre::Real farClipDistance_;
	Ogre::Real nearClipDistance_;
	Ogre::ColourValue backgroundColour_;
};

class CameraFactory : public BaseFactory
{
public:
	CameraFactory() {}
	virtual ~CameraFactory() {}

	virtual Component* create() { return new Camera(); }
};

#endif // !__CAMERA_H__

