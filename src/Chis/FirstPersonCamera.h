#ifndef __FIRST_PERSON_CAMERA_H__
#define __FIRST_PERSON_CAMERA_H__

#include <Engine.h>

class FirstPersonCamera : public Component
{
public:
	FirstPersonCamera();
	virtual ~FirstPersonCamera();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual bool keyPressed(const OIS::KeyEvent &e);
	virtual bool mouseMoved(const OIS::MouseEvent &e);

	inline Ogre::Camera* getCamera() { return camera_; }

private:
	static std::string name_;

	Ogre::Camera* camera_;
	Ogre::Viewport* viewport_;

	Ogre::Real pitchLimit_;
	Ogre::Real farClipDistance_;
	Ogre::Real nearClipDistance_;
	Ogre::ColourValue backgroundColour_;
};

class FirstPersonCameraFactory : public BaseFactory
{
public:
	FirstPersonCameraFactory() {}
	virtual ~FirstPersonCameraFactory() {}

	virtual Component* create() { return new FirstPersonCamera(); }
};

#endif // !__FIRST_PERSON_CAMERA_H__
