#ifndef __FIRST_PERSON_CAMERA_H__
#define __FIRST_PERSON_CAMERA_H__

#include "RenderManager.h"
#include "InputManager.h"

class FirstPersonCamera : public OIS::KeyListener, public OIS::MouseListener
{
public:
	FirstPersonCamera();
	~FirstPersonCamera();

	void update(float deltaTime);

	virtual bool keyPressed(const OIS::KeyEvent &e);
	virtual bool keyReleased(const OIS::KeyEvent &e);
	virtual bool mouseMoved(const OIS::MouseEvent &e);
	virtual bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

private:
	void setCameraAcceleration(float deltaTime);
	void setCameraVelocity(Ogre::Real topSpeed);
	void setBackgroundColour(Ogre::ColourValue color) { viewport_->setBackgroundColour(color); }

	Ogre::Light* light_;
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

#endif // !__FIRST_PERSON_CAMERA_H__
