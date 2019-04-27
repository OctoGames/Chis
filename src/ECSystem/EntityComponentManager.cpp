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

void EntityComponentManager::init()
{
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

void EntityComponentManager::addFactory(const std::string & name, BaseFactory * f)
{
	factories_[name] = f;
}

void EntityComponentManager::addPrototype(Prototype * prototype)
{
	prototypes_[prototype->getName()] = prototype;
}

void EntityComponentManager::addGameObjectWithTag(GameObject* game_object, const std::string& tag)
{
	tags_[game_object->getTag()].push_back(game_object);
}

GameObject * EntityComponentManager::instantiate(const std::string & gameObjectName)
{
	Prototype* prototype = prototypes_[gameObjectName];
	
	GameObject* clonedGameObject = prototype->getEntity()->clone();
	
	for (Component* c : prototype->getComponents())
	{
		Component* clonedComponent = c->clone();
		clonedComponent->setContainer(clonedGameObject);
		clonedComponent->init();
	}

	return clonedGameObject;
}

Component* EntityComponentManager::getComponent(GameObject* game_object, const std::string& component_name)
{
	for (Component* c : containers_[game_object->getGameObjectID()])
	{
		if (c->getName() == component_name) return c;
	}

	return nullptr;
}

Prototype * EntityComponentManager::getPrototype(const std::string & namePrototype)
{
	return prototypes_[namePrototype];
}

std::list<GameObject*> EntityComponentManager::findGameObjectsWithTag(const std::string& tag)
{
	return tags_[tag];
}

GameObject* EntityComponentManager::findGameObjectWithName(const std::string & name)
{
	bool found = false;
	GameObject* auxGO = nullptr;

	std::list<GameObject*>::iterator it = entities_.begin();

	while (it != entities_.end() && !found)
	{
		if ((*it)->getName() == name)
		{
			auxGO = (*it);
			found = true;
		}

		else it++;
	}

	return auxGO;
}
