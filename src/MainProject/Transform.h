#pragma once
#include "Application.h"
#include "component.h"

class Transform : public Component
{
public:
	Transform(GameObject* container, const std::string& parent = "", bool enabled = true);
	virtual ~Transform();

	virtual std::string getName() const { return name_; }

	inline void setPosition(float x, float y, float z) { node_->setPosition(x, y, z); }
	inline void setScale(float x, float y, float z) { node_->setScale(x, y, z); }
	//setVisible

	void attachEntity(Ogre::MovableObject* entity) { node_->attachObject(entity); }

	inline Ogre::SceneNode* getNode() { return node_; }

private:
	static std::string name_;
	Ogre::SceneNode* node_;
};