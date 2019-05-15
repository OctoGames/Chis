#ifndef __GUN_CONTROLLER_H__
#define __GUN_CONTROLLER_H__

#include <Engine.h>


class GunController : public Component
{
public:
	enum GunType { LASER, SHOTGUN, WATERGUN };

	GunController();
	virtual ~GunController();

	virtual void update();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

	inline GunController::GunType getGunType() const { return currentGun_; }

private:
	void shoot();
	void changeGun();
	void reloadGun();

	static std::string name_;

	bool isFiring_;
	OIS::MouseButtonID fireButton_;
	GunController::GunType currentGun_;
	std::vector<std::string> gunMeshes_;
};

class GunControllerFactory : public BaseFactory
{
public:
	GunControllerFactory() {}
	virtual ~GunControllerFactory() {}

	virtual Component* create() { return new GunController(); }
};

#endif // !__GUN_CONTROLLER_H__