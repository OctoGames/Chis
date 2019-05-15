#ifndef __GUN_CONTROLLER_H__
#define __GUN_CONTROLLER_H__

#include <Engine.h>

/*Componente Gun: debe permitir cambiar el mesh del arma (MeshRenderer), la mirilla del HUD (Canvas), 
el sonido que produce (AudioSource) y las balas que instancia (diferentes componentes Bullet); 
tambi�n debe tener datos sobre el arma (n�mero de balas totales/en el cargador) y gestionar la animaci�n de recarga. 
Si no tenemos balas en el cargador, no podremos disparar hasta recargar; si no tenemos balas balas,
no podremos recargar hasta conseguir un cargador.*/

class GunController : public Component
{
public:
	GunController();
	virtual ~GunController();

	virtual void update();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	virtual bool keyPressed(const OIS::KeyEvent &e);

private:
	static std::string name_;

	bool isFiring_;
	OIS::MouseButtonID fireButton_;

	std::list<std::string> listGunsMeshes_;
	OIS::KeyCode gunChangeButton_;

};

class GunControllerFactory : public BaseFactory
{
public:
	GunControllerFactory() {}
	virtual ~GunControllerFactory() {}

	virtual Component* create() { return new GunController(); }
};

#endif // !__GUN_CONTROLLER_H__
/*

*/