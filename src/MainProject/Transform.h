#pragma once
#include "Application.h"
#include "component.h"

class Transform : public Component
{
public:
	Transform(GameObject* container, bool enabled = true);
	virtual ~Transform();

	virtual std::string getName() const { return name_; }

	inline void setPosition(float x, float y, float z) { node_->setPosition(x, y, z); }
	inline void setScale(float x, float y, float z) { node_->setScale(x, y, z); }
	//setVisible

	void attachEntity(Ogre::MovableObject* entity) { node_->attachObject(entity); }

private:
	static std::string name_;
	Ogre::SceneNode* node_;
};