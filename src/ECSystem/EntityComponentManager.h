#ifndef _EC_MANAGER_H_
#define _EC_MANAGER_H_

#include <map>
#include <list>

#include "GameObject.h"
#include "Component.h"

class EntityComponentManager
{
public:
	static EntityComponentManager* Instance();

	void init();
	void update();
	void close();

	void addEntity(GameObject* entity);
	void addComponent(Component* component);
	void addFactory(const std::string& name, BaseFactory* f);
	void addGameObjectWithTag(GameObject* game_object, const std::string& tag);
	BaseFactory* getFactory(const std::string& name) { return factories_[name]; }
	Component* getComponent(GameObject* game_object, const std::string& component_name);
	std::list<GameObject*> findGameObjectsWithTag(const std::string& tag);

private:
	EntityComponentManager();

	static EntityComponentManager* instance_;

	std::list<GameObject*> entities_;
	std::list<Component*> components_;
	std::map<std::string, BaseFactory*> factories_;
	std::map<int, std::list<Component*>> containers_;
	std::map<std::string, std::list<GameObject*>> tags_;
};

#endif