#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <OIS/OISMouse.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>

#include "GameObject.h"

struct ValueType
{
	int i = 0;
	float f = 0.0f;
	bool b = false;
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

	virtual bool keyPressed(const OIS::KeyEvent &e) { return true; }
	virtual bool keyReleased(const OIS::KeyEvent &e) { return true; }
	virtual bool mouseMoved(const OIS::MouseEvent &e) { return true; }
	virtual bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) { return true; }
	virtual bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) { return true; }

	virtual void onCollision(GameObject* gameObject) {}

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

#endif	// !__COMPONENT_H__