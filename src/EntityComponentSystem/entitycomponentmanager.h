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

	void addEntity(GameObject* entity);
	void addComponent(Component* component);
	void addGameObject(GameObject* game_object);
	void addGameObjectWithTag(GameObject* game_object, const std::string& tag);
	Component* getComponent(GameObject* game_object, const std::string& component_name);
	std::list<GameObject*> findGameObjectsWithTag(const std::string& tag);

	void update();
	void close();

private:
	EntityComponentManager();

	static EntityComponentManager* instance_;

	std::list<GameObject*> entities_;
	std::list<Component*> components_;
	std::map<int, std::list<Component*>> containers_;
	std::map<std::string, std::list<GameObject*>> tags_;
};

#endif