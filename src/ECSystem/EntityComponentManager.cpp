#include "EntityComponentManager.h"

bool disposableEntity(GameObject* o) { return o->isDisposable(); }

EntityComponentManager* EntityComponentManager::instance_ = nullptr;

// MANAGER FUNCTIONS ----------------------------------------------------------
EntityComponentManager* EntityComponentManager::Instance()
{
	if (instance_ == nullptr) instance_ = new EntityComponentManager();
	return instance_;
}

EntityComponentManager::EntityComponentManager()
{
}

void EntityComponentManager::init()
{
	std::cout << "\n[ECSystem]: Initializing...\n";
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
	std::cout << "[ECSystem]: Destroying components...\n";
	for (Component* c : components_)
	{
		delete c;
		c = nullptr;
	}
	entities_.clear();

	std::cout << "[ECSystem]: Destroying entities...\n";
	for (GameObject* o : entities_)
	{
		delete o;
		o = nullptr;
	}
	components_.clear();

	for (auto a : prototypes_)
	{
		std::cout << "[ECSystem]: Deregistering " << a.first << " prototype\n";
		delete a.second;
		a.second = nullptr;
	}
	prototypes_.clear();

	for (auto a : factories_)
	{
		std::cout << "[ECSystem]: Deregistering " << a.first << "\n";
		delete a.second;
		a.second = nullptr;
	}
	factories_.clear();

	std::cout << "[ECSystem]: Shutting down...\n\n";
}


// DATA STRUCTURES ------------------------------------------------------------
void EntityComponentManager::addEntity(GameObject* entity)
{
	entities_.push_back(entity);
	if (entity->getTag() != "") tags_[entity->getTag()].push_back(entity);
}

void EntityComponentManager::addComponent(Component* component)
{
	components_.push_back(component);
	containers_[component->gameObject()->getGameObjectID()].push_back(component);
}

void EntityComponentManager::registerFactory(const std::string & factoryName, BaseFactory * f)
{
	std::cout << "[ECSystem]: Registering " << factoryName << "\n";
	factories_[factoryName] = f;
}

void EntityComponentManager::registerPrototype(const std::string & prototypeName, Prototype * prototype)
{
	std::cout << "[ECSystem]: Registering " << prototypeName << "\n";
	prototypes_[prototypeName] = prototype;
}


// LIFE CYCLE FUNCTIONS -------------------------------------------------------
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

void EntityComponentManager::destroy(GameObject * gameObject)
{
	for (Component* c : containers_[gameObject->getGameObjectID()]) c->setEnabled(false);
	gameObject->setDisposable(true);
	gameObject->setActive(false);
}

void EntityComponentManager::clean()
{
	for (GameObject* o : entities_)
	{
		if (o->isDisposable())
		{
			std::string id = o->getGameObjectID();
			std::string tag = o->getTag();

			containers_.erase(id);
			if (tag != "") tags_[tag].remove_if(disposableEntity);
		}
	}
}


// SEARCH FUNCTIONS -----------------------------------------------------------
BaseFactory * EntityComponentManager::getFactory(const std::string & factoryName)
{
	auto it = factories_.find(factoryName);
	if (it == factories_.end())
	{
		std::cout << "[ECSystem]: Cannot find " << factoryName << "!\n";
		return nullptr;
	}
	else return (*it).second;
}

Prototype * EntityComponentManager::getPrototype(const std::string & prototypeName)
{
	auto it = prototypes_.find(prototypeName);
	if (it == prototypes_.end())
	{
		std::cout << "[ECSystem]: Cannot find " << prototypeName << " prototype!\n";
		return nullptr;
	}
	else return (*it).second;
}

GameObject* EntityComponentManager::findGameObjectWithTag(const std::string& gameObjectTag)
{
	auto it = tags_.find(gameObjectTag);
	if (it == tags_.end())
	{
		std::cout << "[ECSystem]: Cannot find " << gameObjectTag << " tag!\n";
		return nullptr;
	}
	else if ((*it).second.empty()) return nullptr;
	else return (*it).second.front();
}

std::list<GameObject*> EntityComponentManager::findGameObjectsWithTag(const std::string& gameObjectTag)
{
	auto it = tags_.find(gameObjectTag);
	if (it == tags_.end())
	{
		std::cout << "[ECSystem]: Cannot find " << gameObjectTag << " tag!\n";
		return std::list<GameObject*>();
	}
	return (*it).second;
}

Component* EntityComponentManager::getComponent(GameObject* gameObject, const std::string& componentName)
{
	std::string id = gameObject->getGameObjectID();
	auto it = containers_.find(id);
	if (it == containers_.end())
	{
		std::cout << "[ECSystem]: Cannot find " << id << " entity!\n";
		return nullptr;
	}
	auto iter = containers_[id].begin();
	while (iter != containers_[id].end() && (*iter)->getName() != componentName) iter++;
	if (iter != containers_[id].end()) return (*iter);
	else return nullptr;
}

std::list<Component*> EntityComponentManager::getComponents(const std::string & gameObjectID)
{
	auto it = containers_.find(gameObjectID);
	if (it == containers_.end())
	{
		std::cout << "[ECSystem]: Cannot find " << gameObjectID << " entity!\n";
		return std::list<Component*>();
	}
	return (*it).second;
}