#include "GunController.h"

#include "Transform.h"
#include "AudioSource.h"
#include "EntityComponentManager.h"

std::string GunController::name_ = "GunController";

GunController::GunController() :
	isFiring_(false)
{
}

GunController::~GunController()
{
}

void GunController::update()
{
	if (isFiring_)
	{
		AudioSource* a = static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"));
		a->play();
		isFiring_ = false;
	}
}

void GunController::init(const std::map<std::string, ValueType>& params)
{
	InputManager::Instance()->addKeyListener(this, "GunController");
	InputManager::Instance()->addMouseListener(this, "GunController");

	setEnabled(params.at("enabled").b);
}

bool GunController::keyPressed(const OIS::KeyEvent & e)
{
	return true;
}

bool GunController::keyReleased(const OIS::KeyEvent & e)
{
	return true;
}

bool GunController::mouseMoved(const OIS::MouseEvent & e)
{
	return true;
}

bool GunController::mousePressed(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	if (id == OIS::MouseButtonID::MB_Left)
	{
		isFiring_ = true;
	}
	return false;
}

bool GunController::mouseReleased(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	return true;
}
