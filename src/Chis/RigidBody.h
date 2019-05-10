#ifndef __RIGIDBODY_H__
#define __RIGIDBODY_H__

#include <Engine.h>
#include "MeshRenderer.h"

class RigidBody : public Component
{
public:
	RigidBody();
	virtual ~RigidBody();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual void onCollision(GameObject* gameObject);

	inline btRigidBody* rigidbody() { return rigidBody_; }

private:
	static std::string name_;
	
	float mass_;
	float radius_;
	Ogre::Vector3 colliderHalfExtent_;
	btRigidBody* rigidBody_;
};

class RigidBodyFactory : public BaseFactory
{
public:
	RigidBodyFactory() {}
	virtual ~RigidBodyFactory() {}

	virtual Component* create() { return new RigidBody(); }
};

#endif // !__RIGIDBODY_H__