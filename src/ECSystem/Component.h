#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <map>

#include "GameObject.h"

struct ValueType
{
	bool b = true;
	float f = 0.0f;
	std::string s = "";
};

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void load(const std::map<std::string, ValueType>& params) = 0;
	virtual std::string getName() const = 0;
	virtual Component* clone() = 0;
	virtual void init() = 0;

	inline GameObject* gameObject() { return container_; }
	inline void setContainer(GameObject* o) { container_ = o; }
	inline bool isEnabled() const { return enabled_; }
	virtual void setEnabled(bool enabled);

	virtual void addCollidedGameObject(GameObject* go) {}
	virtual void clearCollidedGameObjects() {}

	virtual void start() {}
	virtual void fixedUpdate() {}
	virtual void update() {}
	virtual void lateUpdate() {}
	virtual void onEnabled() {}
	virtual void onDisabled() {}
	virtual void onDestroy() {}

	GameObject* container_;
	bool first_enabled_;
	bool enabled_;
};

class BaseFactory
{
public:
	BaseFactory() {}
	virtual ~BaseFactory() {}

	virtual Component* create() = 0;
};

#endif