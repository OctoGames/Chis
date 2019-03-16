#pragma once

#include "OgreSystem.h"
#include "Component.h"

class RigidBody : public Component
{
public:
	RigidBody(GameObject* container, const std::string& parent = "", bool enabled = true);
	virtual ~RigidBody();

	virtual std::string getName() const { return name_; }

	inline Ogre::SceneNode* getNode() { return node_; }

	void createSphereRB(double mass, double radious, std::string name);
	void createBoxRB(double mass, Ogre::Vector3 scale, std::string name);

private:
	static std::string name_;
	Ogre::SceneNode* node_;
};