#ifndef __RAYCAST_BULLET_H__
#define __RAYCAST_BULLET_H__

#include <Engine.h>

class RaycastBullet : public Component
{
public:
	RaycastBullet();
	virtual ~RaycastBullet();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual void start();

private:
	static std::string name_;

	float range_;
	float damage_;
	RaycastHit hit_;
	btVector3 from_;
	btVector3 normal_;
};

class RaycastBulletFactory : public BaseFactory
{
public:
	RaycastBulletFactory() {}
	virtual ~RaycastBulletFactory() {}

	virtual Component* create() { return new RaycastBullet(); }
};

#endif // !__RAYCAST_BULLET_H__
