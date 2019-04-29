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
	inline void setActive(bool active) { active_ = active; }
	inline void setName(const std::string& name) { name_ = name; }

	GameObject* clone();

private:
	Ogre::SceneNode* node_;
	std::string parentTag_;
	std::string name_;
	std::string tag_;
	std::string unique_id_;
	bool active_;

	static int game_object_count_;
};

#endif