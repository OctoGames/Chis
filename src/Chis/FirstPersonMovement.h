#ifndef __FIRST_PERSON_MOVEMENT_H__
#define __FIRST_PERSON_MOVEMENT_H__

#include <Engine.h>

class FirstPersonMovement : public Component
{
public:
	FirstPersonMovement();
	virtual ~FirstPersonMovement();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual void update();

	virtual bool keyPressed(const OIS::KeyEvent &e);
	virtual bool keyReleased(const OIS::KeyEvent &e);

private:
	static std::string name_;

	void setMovementAcceleration(float deltaTime);
	void setMovementVelocity(Ogre::Real topSpeed);

	OIS::KeyCode moveForwardsKey_;
	OIS::KeyCode moveBackwardsKey_;
	OIS::KeyCode moveLeftKey_;
	OIS::KeyCode moveRightKey_;
	OIS::KeyCode fastMoveKey_;

	bool goingForward_;
	bool goingBack_;
	bool goingLeft_;
	bool goingRight_;
	bool fastMove_;

	Ogre::Vector3 velocity_;
	Ogre::Real maxSpeed_;
};

class FirstPersonMovementFactory : public BaseFactory
{
public:
	FirstPersonMovementFactory() {}
	virtual ~FirstPersonMovementFactory() {}

	virtual Component* create() { return new FirstPersonMovement(); }
};

#endif // !__FIRST_PERSON_MOVEMENT_H__
