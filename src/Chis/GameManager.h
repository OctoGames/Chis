#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include <Engine.h>

class GameManager : public Component
{
public:
	GameManager();
	virtual ~GameManager();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

private:
	static std::string name_;
};

class GameManagerFactory : public BaseFactory
{
public:
	GameManagerFactory() {}
	virtual ~GameManagerFactory() {}

	virtual Component* create() { return new GameManager(); }
};

#endif // !__GAME_MANAGER_H__
