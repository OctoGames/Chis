#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <Engine.h>

class Player : public Component
{
public:
	Player();
	virtual ~Player();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual void onCollision(GameObject* other);
	virtual void update();

	void damage(float dmg) { health_ -= dmg; }

private:
	static std::string name_;

	float health_;
	bool invulnerable_;
	Ogre::Timer* invulnerabilityTimer_;
};

class PlayerFactory : public BaseFactory
{
public:
	PlayerFactory() {}
	virtual ~PlayerFactory() {}

	virtual Component* create() { return new Player(); }
};

#endif // !__PLAYER_H__
