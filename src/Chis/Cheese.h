#ifndef __CHEESE_H__
#define __CHEESE_H__

#include <Engine.h>

class Cheese : public Component
{
public:
	Cheese();
	virtual ~Cheese();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual void onCollision(GameObject* other);
	virtual void update();

	void damage(float dmg) { cheesyness_ -= dmg; }

private:
	static std::string name_;

	float cheesyness_;
};

class CheeseFactory : public BaseFactory
{
public:
	CheeseFactory() {}
	virtual ~CheeseFactory() {}

	virtual Component* create() { return new Cheese(); }
};

#endif // !__CHEESE_H__
