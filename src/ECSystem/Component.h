#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "GameObject.h"

class Component
{
public:
	Component(GameObject* container, bool enabled = true);
	virtual ~Component();

	virtual std::string getName() const = 0;

	inline GameObject* gameObject() { return container_; }
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