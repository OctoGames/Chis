#ifndef __PLAYER_CONTROLLER_H__
#define __PLAYER_CONTROLLER_H__

#include <Engine.h>

class PlayerController : public Component
{
public:
	PlayerController();
	virtual ~PlayerController();

	virtual void start();
	virtual void fixedUpdate();

	virtual void onCollision(GameObject* other);

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual bool keyPressed(const OIS::KeyEvent &e);
	virtual bool keyReleased(const OIS::KeyEvent &e);

	void setPlayerAcceleration(float deltaTime);
	void setPlayerVelocity(Ogre::Real topSpeed);

private:
	static std::string name_;

	btRigidBody* rigidbody_;

	Ogre::Real maxSpeed_;
	Ogre::Vector3 velocity_;

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
};

class PlayerControllerFactory : public BaseFactory
{
public:
	PlayerControllerFactory() {}
	virtual ~PlayerControllerFactory() {}

	virtual Component* create() { return new PlayerController(); }
};

#endif // !__PLAYER_CONTROLLER_H__