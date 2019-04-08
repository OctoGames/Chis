#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "RenderManager.h"
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	Transform(GameObject* container, const std::string& parent = "", bool enabled = true);
	virtual ~Transform();

	virtual void init(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }

	inline Ogre::SceneNode* getNode() { return node_; }
	inline void setPosition(float x, float y, float z) { node_->setPosition(x, y, z); }
	inline void setPosition(const Ogre::Vector3 &v) { node_->setPosition(v); }
	inline void setDirection(float x, float y, float z) { node_->setDirection(x, y, z); }
	inline void setDirection(const Ogre::Vector3 &v) { node_->setDirection(v); }
	inline void setScale(float x, float y, float z) { node_->setScale(x, y, z); }
	inline void setScale(const Ogre::Vector3 &v) { node_->setScale(v); }
	inline void setOrientation(const Ogre::Quaternion &q) { node_->setOrientation(q); };
	inline void rotate(const Ogre::Vector3& axis, float degrees) { node_->rotate(axis, Ogre::Radian(Ogre::Degree(degrees))); }

	void attachEntity(Ogre::MovableObject* entity) { node_->attachObject(entity); }

private:
	static std::string name_;
	Ogre::SceneNode* node_;
};

class TransformFactory : public BaseFactory
{
public:
	TransformFactory() {}
	virtual ~TransformFactory() {}

	virtual Component* create() { return new Transform(); }
};

#endif // !__TRANSFORM_H__