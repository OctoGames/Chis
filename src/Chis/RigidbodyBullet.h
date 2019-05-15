#ifndef __RIGIDBODY_BULLET_H__
#define __RIGIDBODY_BULLET_H__

#include <Engine.h>

class RigidbodyBullet : public Component
{
public:
	RigidbodyBullet();
	virtual ~RigidbodyBullet();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual void onCollision(GameObject* other);
	virtual void start();

private:
	static std::string name_;

	float range_;
	float speed_;
	float damage_;
	Ogre::Vector3 initialPosition_;
	Ogre::ParticleSystem* particles_;
};

class RigidbodyBulletFactory : public BaseFactory
{
public:
	RigidbodyBulletFactory() {}
	virtual ~RigidbodyBulletFactory() {}

	virtual Component* create() { return new RigidbodyBullet(); }
};

#endif // !__RIGIDBODY_BULLET_H__
