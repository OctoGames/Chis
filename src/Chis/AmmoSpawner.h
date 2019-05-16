#ifndef __AMMO_SPAWNER_H__
#define __AMMO_SPAWNER_H__

#include <Engine.h>

class AmmoSpawner : public Component
{
public:
	AmmoSpawner();
	virtual ~AmmoSpawner();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual void update();

private:
	void spawnAmmo();
	
	static std::string name_;

	Ogre::Timer* spawnTimer_; 
	float spawnTime_;
};

class AmmoSpawnerFactory : public BaseFactory
{
public:
	AmmoSpawnerFactory() {}
	virtual ~AmmoSpawnerFactory() {}

	virtual Component* create() { return new AmmoSpawner(); }
};

#endif // !__ENEMY_SPAWNER_H__
