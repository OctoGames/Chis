#ifndef __AMMO_H__
#define __AMMO_H__

#include <Engine.h>

class Ammo : public Component
{
public:
	Ammo();
	virtual ~Ammo();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual void onCollision(GameObject* other);
	virtual void update();

	inline int getNumBullets() const { return numBullets_; }
	inline void setNumBullets(int bullets) { numBullets_ = bullets; }

private:
	static std::string name_;

	int numBullets_;
};

class AmmoFactory : public BaseFactory
{
public:
	AmmoFactory() {}
	virtual ~AmmoFactory() {}

	virtual Component* create() { return new Ammo(); }
};

#endif // !__AMMO_H__
