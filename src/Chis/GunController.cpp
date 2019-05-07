#include "GunController.h"

#include "RigidBody.h"
#include "AudioSource.h"
#include "FirstPersonCamera.h"

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
	enabled_ = params.at("enabled_gc").b;
	//fireButton_ = static_cast<OIS::MouseButtonID>(params.at("fire_button").i);
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

bool GunController::mousePressed(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	if (id == OIS::MouseButtonID::MB_Left)
	{
		isFiring_ = true;

		GameObject* player = EntityComponentManager::Instance()->findGameObjectWithTag("Player");
		Ogre::Camera* cam = static_cast<FirstPersonCamera*>(EntityComponentManager::Instance()->getComponent(player, "FirstPersonCamera"))->getCamera();
		
		Ogre::Vector3 f = cam->getRealPosition();
		Ogre::Vector3 n = cam->getRealDirection();
		Ogre::Quaternion q = cam->getRealOrientation();
		btVector3 from(f.x, f.y, f.z);
		btVector3 normal(n.x, n.y, n.z);		
		
		GameObject* bullet = EntityComponentManager::Instance()->instantiate("Bullet", f, q);

		RigidBody* rb = static_cast<RigidBody*>(EntityComponentManager::Instance()->getComponent(bullet, "RigidBody"));
		rb->rigidbody()->applyCentralImpulse(normal * 1000.0f);

		RaycastHit hit;
		if (Physics::Instance()->raycast(from, normal, 1000.0f, &hit))
		{
			std::cout << hit.gameObject->getName() << std::endl;
		}
	}

	return true;
}