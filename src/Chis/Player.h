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

	inline int getLife() const { return life_; }
	inline void setLife(int life) { life_ = life; }

	void damage(int dmg) { life_ -= dmg; }

private:
	static std::string name_;

	int life_;
	int bullets_;
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

#endif // !__GAME_MANAGER_H__
