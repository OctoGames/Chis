#ifndef __EC_MANAGER_H__
#define __EC_MANAGER_H__

#include <map>

#include "Prototype.h"

struct comp {
	bool operator() (const std::string& lhs, const std::string& rhs) const {
		return stricmp(lhs.c_str(), rhs.c_str()) < 0;
	}
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

	void registerPrototype(const std::string& prototypeName, Prototype* prototype);
	void registerFactory(const std::string& factoryName, BaseFactory* f);

	Component* getComponent(GameObject* gameObject, const std::string& componentName);
	GameObject* instantiate(const std::string& prototypeName, const Ogre::Vector3& position = Ogre::Vector3::ZERO, const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY);

	BaseFactory* getFactory(const std::string& factoryName) { return factories_[factoryName]; }
	Prototype* getPrototype(const std::string& prototypeName) { return prototypes_[prototypeName]; }
	std::list<Component*> getComponents(const std::string& gameObjectID) { return containers_[gameObjectID]; }
	std::list<GameObject*> findGameObjectsWithTag(const std::string& gameObjectTag) { return tags_[gameObjectTag]; }
	GameObject* findGameObjectWithTag(const std::string& gameObjectTag) { return tags_[gameObjectTag].front(); }

private:
	EntityComponentManager();

	static EntityComponentManager* instance_;

	std::list<Component*> components_;
	std::map<std::string, Prototype*, comp> prototypes_;
	std::map<std::string, BaseFactory*, comp> factories_;
	std::map<std::string, std::list<Component*>, comp> containers_;
	std::map<std::string, std::list<GameObject*>, comp> tags_;
};

#endif	// !__EC_MANAGER_H__