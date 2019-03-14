#ifndef _TRANSFROM_H_
#define _TRANSFORM_H_

#include "OgreSystem.h"
#include "Component.h"

class Transform : public Component
{
public:
	Transform(GameObject* container, const std::string& parent = "", bool enabled = true);
	virtual ~Transform();

	virtual std::string getName() const { return name_; }

	inline Ogre::SceneNode* getNode() { return node_; }
	inline void setPosition(float x, float y, float z) { node_->setPosition(x, y, z); }
	inline void setScale(float x, float y, float z) { node_->setScale(x, y, z); }

	void attachEntity(Ogre::MovableObject* entity) { node_->attachObject(entity); }	

private:
	static std::string name_;
	Ogre::SceneNode* node_;
};

#endif // !_TRANSFROM_H_