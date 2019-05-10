#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "RenderManager.h"

class GameObject
{
public:
	GameObject(const std::string& name, const std::string& parentTag = "", const std::string& tag = "", bool active = true);
	~GameObject();

	inline Ogre::SceneNode* transform() { return node_; }
	inline std::string getParent() const { return parentTag_; }
	inline std::string getName() const { return name_; }
	inline std::string getTag() const { return tag_; }
	inline std::string getGameObjectID() const { return unique_id_; }
	inline bool isActive() const { return active_; }
	void setActive(bool active);
	inline bool isDisposable() const { return disposable_; }
	inline void setDisposable(bool disposable) { disposable_ = disposable; }
	inline void setName(const std::string& name) { name_ = name; }
	inline bool isDestroyable() const { return !dontDestroyOnLoad_; }
	inline void setDestroyable(bool destroyable) { dontDestroyOnLoad_ == !destroyable; }
	GameObject* clone();

private:
	Ogre::SceneNode* node_;
	std::string parentTag_;
	std::string name_;
	std::string tag_;
	std::string unique_id_;
	bool active_;
	bool disposable_;
	bool dontDestroyOnLoad_;

	static int game_object_count_;
};

#endif