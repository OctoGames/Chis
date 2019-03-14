#include "EntityComponentManager.h"

EntityComponentManager* EntityComponentManager::instance_ = nullptr;

EntityComponentManager::EntityComponentManager()
{
}

EntityComponentManager* EntityComponentManager::Instance()
{
	if (instance_ == nullptr) instance_ = new EntityComponentManager();
	return instance_;
}

void EntityComponentManager::update()
{
	for (Component* c : components_)
	{
		c->fixedUpdate();
		c->update();
		c->lateUpdate();
	}
}

void EntityComponentManager::close()
{
	for (GameObject* o : entities_)
	{
		delete o;
		o = nullptr;
	}

	for (Component* c : components_)
	{
		delete c;
		c = nullptr;
	}
}

void EntityComponentManager::addEntity(GameObject* entity)
{
	entities_.push_back(entity);
}

void EntityComponentManager::addComponent(Component* component)
{
	components_.push_back(component);
	containers_[component->gameObject()->getGameObjectID()].push_back(component);
}

void EntityComponentManager::addGameObject(GameObject* game_object)
{
	containers_[game_object->getGameObjectID()] = std::list<Component*>();
}

void EntityComponentManager::addGameObjectWithTag(GameObject* game_object, const std::string& tag)
{
	tags_[game_object->getTag()].push_back(game_object);
}

Component* EntityComponentManager::getComponent(GameObject* game_object, const std::string& component_name)
{
	for (Component* c : containers_[game_object->getGameObjectID()])
	{
		if (c->getName() == component_name) return c;
	}

	return nullptr;
}

std::list<GameObject*> EntityComponentManager::findGameObjectsWithTag(const std::string& tag)
{
	return tags_[tag];
}
