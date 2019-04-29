#ifndef __RIGIDBODY_H__
#define __RIGIDBODY_H__

#include "Component.h"
#include "GameObject.h"
#include "Physics.h"

class RigidBody : public Component
{
public:
	RigidBody();
	virtual ~RigidBody();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual void fixedUpdate();

	virtual void addCollidedGameObject(GameObject* go) { collidedGameObjects_.push_back(go); };
	virtual void clearCollidedGameObjects() { collidedGameObjects_.clear(); };

	inline btRigidBody* rigidbody() { return rigidBody_; }

private:
	static std::string name_;

	double mass_;
	double radius_;
	Ogre::Vector3 scale_;
	btRigidBody* rigidBody_;
	std::list<GameObject*> collidedGameObjects_;
};

class RigidBodyFactory : public BaseFactory
{
public:
	RigidBodyFactory() {}
	virtual ~RigidBodyFactory() {}

	virtual Component* create() { return new RigidBody(); }
};

#endif // !__RIGIDBODY_H__