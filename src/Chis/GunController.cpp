#include "GunController.h"

#include "RigidBody.h"
#include "AudioSource.h"
#include "FirstPersonCamera.h"
#include "Canvas.h"


std::string GunController::name_ = "GunController";

GunController::GunController() :
	isFiring_(false),
	totalBullets_(40),
	remainingBullets_(20),
	fireButton_(OIS::MouseButtonID::MB_Left),
	currentGun_(GunController::GunType::LASER),
	magazineSize_(20),
	numBulletsLaser_(1),
	numBulletsShotgun_(5)
{
}

GunController::~GunController()
{
}

void GunController::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_gc");
	if (it != params.end()) enabled_ = params.at("enabled_gc").b;

	int i = 0;
	std::stringstream s;
	s << "mesh_name" << i;
	it = params.find(s.str());
	while (it != params.end())
	{
		gunMeshes_.push_back((*it).second.s);
		i++;
		std::stringstream s1;
		s1 << "mesh_name" << i;
		it = params.find(s1.str());
	}
}

Component * GunController::clone()
{
	GunController* clonedComponent = new GunController();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->fireButton_ = this->fireButton_;

	return clonedComponent;
}

void GunController::init()
{
	InputManager::Instance()->addMouseListener(this, "GunController");
	InputManager::Instance()->addKeyListener(this, "GunController");
	setEnabled(enabled_);
}

bool GunController::keyPressed(const OIS::KeyEvent & e)
{
	if (e.key == OIS::KC_R)
	{
		reloadGun();
	}

	return true;
}

bool GunController::mousePressed(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	if (id == OIS::MouseButtonID::MB_Left) shoot();
	else if (id == OIS::MouseButtonID::MB_Right) changeGun();
	return true;
}

bool GunController::mouseReleased(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	if (id == OIS::MouseButtonID::MB_Left) isFiring_ = false;
	return true;
}

void GunController::start()
{
	Canvas* canvas = static_cast<Canvas*>(EntityComponentManager::Instance()->getComponent("GameManager", "Canvas"));
	if (canvas) canvas->updateBullets(remainingBullets_, totalBullets_);

}


void GunController::shoot()
{
	if (remainingBullets_ == 0)
	{
		if (totalBullets_ == 0) return;
		else
		{
			reloadGun();
			return;
		}
	}

	Ogre::Camera* cam = static_cast<FirstPersonCamera*>(EntityComponentManager::Instance()->getComponent("Player", "FirstPersonCamera"))->getCamera();

	if (cam)
	{
		isFiring_ = true;

		Ogre::Vector3 f = cam->getRealPosition();
		Ogre::Vector3 n = cam->getRealDirection();
		Ogre::Quaternion q = cam->getRealOrientation();
		f = f + n * 10;

		switch (currentGun_)
		{
		case GunController::LASER:
			if (remainingBullets_ >= numBulletsLaser_)
			{
				EntityComponentManager::Instance()->instantiate("RaycastBullet", f, q);
				remainingBullets_ -= numBulletsLaser_;
			}
			break;

		case GunController::SHOTGUN:
			if (remainingBullets_ >= numBulletsShotgun_)
			{
				EntityComponentManager::Instance()->instantiate("RigidbodyBullet", f, q);
				remainingBullets_ -= numBulletsShotgun_;
			}
			break;

		case GunController::WATERGUN:
			if (remainingBullets_ >= numBulletsLaser_)
			{
				EntityComponentManager::Instance()->instantiate("RaycastBullet", f, q);
				remainingBullets_ -= numBulletsLaser_;
			}
			break;
		}

		Canvas* canvas = static_cast<Canvas*>(EntityComponentManager::Instance()->getComponent("GameManager", "Canvas"));
		if (canvas) canvas->updateBullets(remainingBullets_, totalBullets_);
	}
}

void GunController::reloadGun()
{
	int bulletsToReload = magazineSize_ - remainingBullets_;
	if (totalBullets_ < bulletsToReload) bulletsToReload = totalBullets_;
	remainingBullets_ += bulletsToReload;
	totalBullets_ -= bulletsToReload;

	AudioSource* as = static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"));
	if (as) as->play();

	Canvas* canvas = static_cast<Canvas*>(EntityComponentManager::Instance()->getComponent("GameManager", "Canvas"));
	if (canvas) canvas->updateBullets(remainingBullets_, totalBullets_);
}

void GunController::changeGun()
{
	reloadGun();

	switch (currentGun_)
	{
	case GunController::LASER:
		currentGun_ = GunController::SHOTGUN;
		break;
	case GunController::SHOTGUN:
		currentGun_ = GunController::WATERGUN;
		break;
	case GunController::WATERGUN:
		currentGun_ = GunController::LASER;
		break;
	}

	Canvas* canvas = static_cast<Canvas*>(EntityComponentManager::Instance()->getComponent("GameManager", "Canvas"));
	if (canvas) canvas->changeReticle();

	MeshRenderer* mr = static_cast<MeshRenderer*>(EntityComponentManager::Instance()->getComponent(gameObject(), "MeshRenderer"));
	if (mr) mr->changeMesh(gunMeshes_[currentGun_], "");
}

void GunController::addBullets(int numBullets)
{
	totalBullets_ += numBullets;

	AudioSource* as = static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"));
	if (as) as->play();

	Canvas* canvas = static_cast<Canvas*>(EntityComponentManager::Instance()->getComponent("GameManager", "Canvas"));
	if (canvas) canvas->updateBullets(remainingBullets_, totalBullets_);
}

