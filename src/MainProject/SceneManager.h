#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Component.h"

class SceneManager : public Component
{
public:
	SceneManager();
	virtual ~SceneManager();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	void createMenuScene();
	void createGameScene();

private:
	static std::string name_;
};

class SceneManagerFactory : public BaseFactory
{
public:
	SceneManagerFactory() {}
	virtual ~SceneManagerFactory() {}

	virtual Component* create() { return new SceneManager(); }
};

#endif // !__SCENE_MANAGER_H__
