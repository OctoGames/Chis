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
	for (Component* c : components_)
	{
		delete c;
		c = nullptr;
	}
}

void EntityComponentManager::addEntity(GameObject* entity)
{
	if (entity->getTag() != "") tags_[entity->getTag()].push_back(entity);
}

void EntityComponentManager::addComponent(Component* component)
{
	components_.push_back(component);
	containers_[component->gameObject()->getGameObjectID()].push_back(component);
}

void EntityComponentManager::registerFactory(const std::string & factoryName, BaseFactory * f)
{
	factories_[factoryName] = f;
}

void EntityComponentManager::registerPrototype(const std::string & prototypeName, Prototype * prototype)
{
	prototypes_[prototypeName] = prototype;
}

GameObject * EntityComponentManager::instantiate(const std::string & gameObjectName, const Ogre::Vector3& position, const Ogre::Quaternion& orientation)
{
	Prototype* prototype = prototypes_[gameObjectName];
	
	GameObject* clonedGameObject = prototype->getEntity()->clone();
	clonedGameObject->transform()->setPosition(position);
	clonedGameObject->transform()->setOrientation(orientation);
	
	for (Component* c : prototype->getComponents())
	{
		Component* clonedComponent = c->clone();
		clonedComponent->setContainer(clonedGameObject);
		clonedComponent->init();
	}

	return clonedGameObject;
}

Component* EntityComponentManager::getComponent(GameObject* gameObject, const std::string& componentName)
{
	for (Component* c : containers_[gameObject->getGameObjectID()])
	{
		if (c->getName() == componentName) return c;
	}

	return nullptr;
}