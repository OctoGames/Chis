#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <Engine.h>

class Enemy : public Component
{
public:
	Enemy();
	virtual ~Enemy();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual void onCollision(GameObject* other);
	virtual void update();

	inline float getDamage() const { return damage_; }
	void damage(float dmg) { health_ -= dmg; }

private:
	static std::string name_;

	float health_;
	float damage_;
	float score_;
};

class EnemyFactory : public BaseFactory
{
public:
	EnemyFactory() {}
	virtual ~EnemyFactory() {}

	virtual Component* create() { return new Enemy(); }
};

#endif // !__ENEMY_H__
