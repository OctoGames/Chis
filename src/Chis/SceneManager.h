#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include <Engine.h>

class SceneManager : public Component
{
public:
	SceneManager();
	virtual ~SceneManager();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual void start();

	void createMenuScene();
	void createGameScene();
	void createEndScene();

private:
	static std::string name_;

	void clearScene();
};

class SceneManagerFactory : public BaseFactory
{
public:
	SceneManagerFactory() {}
	virtual ~SceneManagerFactory() {}

	virtual Component* create() { return new SceneManager(); }
};

#endif // !__SCENE_MANAGER_H__