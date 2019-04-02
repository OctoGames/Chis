#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <string>

class GameObject
{
public:
	GameObject(const std::string& name, const std::string& tag = "", bool active = true);
	~GameObject();

	inline std::string getName() const { return name_; }
	inline std::string getTag() const { return tag_; }
	inline int getGameObjectID() const { return unique_id_; }
	inline bool isActive() const { return active_; }
	inline void setActive(bool active) { active_ = active; }

private:
	std::string name_;
	std::string tag_;
	int unique_id_;
	bool active_;

	static int game_object_count_;
};

#endif