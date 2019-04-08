#ifndef __GUN_CONTROLLER_H__
#define __GUN_CONTROLLER_H__

#include "InputManager.h"
#include "Component.h"

class GunController : public Component, public OIS::KeyListener, public OIS::MouseListener
{
public:
	GunController();
	virtual ~GunController();

	virtual void update();

	virtual void init(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }

	virtual bool keyPressed(const OIS::KeyEvent &e);
	virtual bool keyReleased(const OIS::KeyEvent &e);
	virtual bool mouseMoved(const OIS::MouseEvent &e);
	virtual bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

private:
	static std::string name_;

	bool isFiring_;
};

class GunControllerFactory : public BaseFactory
{
public:
	GunControllerFactory() {}
	virtual ~GunControllerFactory() {}

	virtual Component* create() { return new GunController(); }
};

#endif // !__GUN_CONTROLLER_H__
