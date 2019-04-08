#pragma once

#include "RenderManager.h"
#include "Component.h"
#include "GameObject.h"

class RigidBody : public Component
{
public:
	RigidBody();
	RigidBody(GameObject* container, const std::string& parent = "", bool enabled = true);
	virtual ~RigidBody();

	virtual void init(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }

	inline Ogre::SceneNode* getNode() { return node_; }

	void createSphereRB(double mass, double radious, std::string name);
	void createBoxRB(double mass, Ogre::Vector3 scale, std::string name);

	virtual inline void addCollidedGameObject(GameObject* go) { collidedGameObjects_.push_back(go);};
	virtual inline void clearCollidedGameObjects() { collidedGameObjects_.clear(); };

private:
	static std::string name_;
	Ogre::SceneNode* node_;

	std::vector<GameObject*> collidedGameObjects_;
};

class RigidBodyFactory : public BaseFactory
{
public:
	RigidBodyFactory() {}
	virtual ~RigidBodyFactory() {}

	virtual Component* create() { return new RigidBody(); }
};