#ifndef _EC_MANAGER_H_
#define _EC_MANAGER_H_

#include <map>
#include <list>

#include "gameobject.h"
#include "component.h"

class EntityComponentManager
{
public:
	EntityComponentManager* Instance();

	void addComponent(Component* component)
	{
		components_.push_back(component);
	}

	void addGameObject(GameObject* game_object)
	{
		containers_[game_object->getGameObjectID()] = std::list<Component*>();
	} 

	void addGameObjectWithTag(GameObject* game_object, const std::string& tag)
	{
		tags_[game_object->getTag()].push_back(game_object);
	}

	Component* getComponent(GameObject* game_object, const std::string& component_name)
	{
		for (Component* c : containers_[game_object->getGameObjectID()])
		{
			if (c->getName() == component_name) return c;
		}

		return nullptr;
	}

	std::list<GameObject*> findGameObjectsWithTag(const std::string& tag)
	{
		return tags_[tag];
	}

	void tick()
	{
		for (Component* c : components_)
		{
			c->fixedUpdate();
			c->update();
			c->lateUpdate();
		}
	}

private:
	EntityComponentManager();

	static EntityComponentManager* instance_;

	std::list<Component*> components_;
	std::map<int, std::list<Component*>> containers_;
	std::map<std::string, std::list<GameObject*>> tags_;
};

#endif