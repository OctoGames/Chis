#ifndef __ENEMY_SPAWNER_H__
#define __ENEMY_SPAWNER_H__

#include <Engine.h>

class EnemySpawner : public Component
{
public:
	EnemySpawner();
	virtual ~EnemySpawner();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual void update();

private:
	void spawnEnemy();
	
	static std::string name_;

	Ogre::Timer* spawnTimer_; 
	float spawnTime_;
};

class EnemySpawnerFactory : public BaseFactory
{
public:
	EnemySpawnerFactory() {}
	virtual ~EnemySpawnerFactory() {}

	virtual Component* create() { return new EnemySpawner(); }
};

#endif // !__ENEMY_SPAWNER_H__
