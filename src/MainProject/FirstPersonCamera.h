#ifndef __FIRST_PERSON_CAMERA_H__
#define __FIRST_PERSON_CAMERA_H__

#include "RenderManager.h"
#include "InputManager.h"
#include "Component.h"

class FirstPersonCamera : public Component, public OIS::KeyListener, public OIS::MouseListener
{
public:
	FirstPersonCamera();
	FirstPersonCamera(GameObject* container, bool enabled);
	virtual ~FirstPersonCamera();

	virtual void update();

	virtual void init(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }

	virtual bool keyPressed(const OIS::KeyEvent &e);
	virtual bool keyReleased(const OIS::KeyEvent &e);
	virtual bool mouseMoved(const OIS::MouseEvent &e);
	virtual bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

private:
	void setCameraAcceleration(float deltaTime);
	void setCameraVelocity(Ogre::Real topSpeed);
	void setBackgroundColour(Ogre::ColourValue color) { viewport_->setBackgroundColour(color); }

	static std::string name_;

	Ogre::Camera* camera_;
	Ogre::Viewport* viewport_;
	Ogre::SceneNode* cameraNode_;

	Ogre::Real maxSpeed_;
	Ogre::Real pitchLimit_;
	Ogre::Vector3 velocity_;

	bool changeBackground_;
	bool goingForward_;
	bool goingBack_;
	bool goingLeft_;
	bool goingRight_;
	bool fastMove_;
};

class FirstPersonCameraFactory : public BaseFactory
{
public:
	FirstPersonCameraFactory() {}
	virtual ~FirstPersonCameraFactory() {}

	virtual Component* create() { return new FirstPersonCamera(); }
};

#endif // !__FIRST_PERSON_CAMERA_H__
