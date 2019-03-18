#pragma once

#include "OgreSystem.h"
#include "Component.h"

class Transform : public Component
{
public:
	Transform(GameObject* container, const std::string& parent = "", bool enabled = true);
	virtual ~Transform();

	virtual std::string getName() const { return name_; }

	inline void setPosition(float x, float y, float z) { node_->setPosition(x, y, z); }
	inline void setPosition(const Ogre::Vector3 &v) { node_->setPosition(v); }
	inline void setScale(float x, float y, float z) { node_->setScale(x, y, z); }
	inline void setScale(const Ogre::Vector3 &v) { node_->setScale(v); }
	inline void setOrientation(const Ogre::Quaternion &q) { node_->setOrientation(q); };
	//setVisible

	void attachEntity(Ogre::MovableObject* entity) { node_->attachObject(entity); }

	inline Ogre::SceneNode* getNode() { return node_; }

private:
	static std::string name_;
	Ogre::SceneNode* node_;
};