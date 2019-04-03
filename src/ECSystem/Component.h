#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <map>

#include "GameObject.h"

struct ValueType
{
	int i = 0;
	float f = 0.0f;
	double d = 0.0;
	bool b = false;
	char c = ' ';
	std::string s = "";
};

class Component
{
public:
	Component();
	Component(GameObject* container, bool enabled = true);
	virtual ~Component();

	virtual void init(const std::map<std::string, ValueType>& params) = 0;
	virtual std::string getName() const = 0;

	inline GameObject* gameObject() { return container_; }
	inline void setContainer(GameObject* o) { container_ = o; }
	inline bool isEnabled() const { return enabled_; }
	inline void setEnabled(bool enabled);

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