#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "RenderManager.h"

class GameObject
{
public:
	GameObject(const std::string& name, const std::string& parentTag = "", const std::string& tag = "", bool active = true);
	~GameObject();

	inline Ogre::SceneNode* transform() { return node_; }

	inline void setName(const std::string& name) { name_ = name; }
	inline std::string getName() const { return name_; }
	inline std::string getGameObjectID() const { return uniqueId_; }
	inline std::string getTag() const { return tag_; }
	inline std::string getParent() const { return parentTag_; }

	inline bool isDisposable() const { return disposable_; }
	inline void setDisposable(bool disposable) { disposable_ = disposable; }
	inline bool isDestroyable() const { return destroyable_; }
	inline void setDestroyable(bool destroyable) { destroyable_ = destroyable; }
	inline bool isActive() const { return active_; }
	void setActive(bool active);

	GameObject* clone();

private:
	static int gameObjectId_;

	Ogre::SceneNode* node_;
	
	std::string name_;
	std::string uniqueId_;
	std::string tag_;

	std::string parentTag_;

	bool active_;
	bool disposable_;
	bool destroyable_;
};

#endif