#include "GunController.h"

#include "AudioSource.h"
#include "EntityComponentManager.h"

std::string GunController::name_ = "GunController";

GunController::GunController() :
	isFiring_(false),
	fireButton_(OIS::MouseButtonID::MB_Left)
{
}

GunController::~GunController()
{
}

void GunController::update()
{
	if (isFiring_)
	{
		static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"))->play();
		isFiring_ = false;
	}
}

void GunController::load(const std::map<std::string, ValueType>& params)
{
	//enabled_ = params.at("enabled").b;
	fireButton_ = (OIS::MouseButtonID)(int)params.at("fire_button").f;
}

Component * GunController::clone()
{
	GunController* clonedComponent = new GunController();

	clonedComponent->enabled_ = this->enabled_;
	fireButton_ = this->fireButton_;

	return clonedComponent;
}

void GunController::init()
{
	InputManager::Instance()->addMouseListener(this, "GunController");
	setEnabled(enabled_);
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
