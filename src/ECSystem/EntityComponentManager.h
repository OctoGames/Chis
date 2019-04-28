#ifndef _EC_MANAGER_H_
#define _EC_MANAGER_H_

#include <map>
#include <list>

#include "GameObject.h"
#include "Component.h"

class Prototype
{
public:
	Prototype(const std::string& name, GameObject* container, const std::list<Component*>& components) : 
		name_(name), container_(container), components_(components) {}
	~Prototype() {}

	inline std::string getName() const { return name_; }
	inline GameObject* getEntity() { return container_; }
	inline std::list<Component*> getComponents() { return components_; }

private:
	std::string name_;
	GameObject* container_;
	std::list<Component*> components_;
};

class EntityComponentManager
{
public:
	static EntityComponentManager* Instance();

	void init();
	void update();
	void close();

	void addEntity(GameObject* entity);
	void addComponent(Component* component);
	void addPrototype(Prototype* prototype);
	void addFactory(const std::string& name, BaseFactory* f);
	void addGameObjectWithTag(GameObject* game_object, const std::string& tag);

	GameObject* instantiate(const std::string& gameObjectName);

	BaseFactory* getFactory(const std::string& name) { return factories_[name]; }
	Component* getComponent(GameObject* game_object, const std::string& component_name);
	Prototype* getPrototype(const std::string& namePrototype);
	std::list<GameObject*> findGameObjectsWithTag(const std::string& tag);
	
	GameObject* findGameObjectWithName(const std::string& name);
	GameObject* findGameObjectWithID(const std::string & name);

private:
	EntityComponentManager();

	static EntityComponentManager* instance_;

	std::list<GameObject*> entities_;
	std::list<Component*> components_;
	std::map<std::string, BaseFactory*> factories_;
	std::map<std::string, Prototype*> prototypes_;
	std::map<std::string, std::list<Component*>> containers_;
	std::map<std::string, std::list<GameObject*>> tags_;
};

#endif