#include "RigidbodyBullet.h"

#include "Enemy.h"
#include "RigidBody.h"
#include "AudioSource.h"
#include "FirstPersonCamera.h"

std::string RigidbodyBullet::name_ = "RigidbodyBullet";

RigidbodyBullet::RigidbodyBullet() :
	range_(1000.0f),
	speed_(1000.0f),
	damage_(20.0f),
	initialPosition_(Ogre::Vector3::ZERO)
{
}

RigidbodyBullet::~RigidbodyBullet()
{
}

void RigidbodyBullet::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_rbb"); if (it != params.end()) enabled_ = params.at("enabled_rbb").b;
	it = params.find("range"); if (it != params.end()) range_ = params.at("range").f;
	it = params.find("damage"); if (it != params.end()) damage_ = params.at("damage").f;
	it = params.find("speed"); if (it != params.end()) speed_ = params.at("speed").f;
}

Component * RigidbodyBullet::clone()
{
	RigidbodyBullet* clonedComponent = new RigidbodyBullet();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->range_ = this->range_;
	clonedComponent->damage_ = this->damage_;
	clonedComponent->speed_ = this->speed_;

	return clonedComponent;
}

void RigidbodyBullet::init()
{
	initialPosition_ = gameObject()->transform()->getPosition();
	setEnabled(enabled_);
}

void RigidbodyBullet::onCollision(GameObject * other)
{
	if (other->getTag() != "player")
	{
		static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"))->play();
		EntityComponentManager::Instance()->destroy(gameObject());
	}
}

void RigidbodyBullet::start()
{
	Ogre::Camera* cam = static_cast<FirstPersonCamera*>(EntityComponentManager::Instance()->getComponent("Player", "FirstPersonCamera"))->getCamera();

	if (cam)
	{
		Ogre::Vector3 f = cam->getRealPosition();
		Ogre::Vector3 n = cam->getRealDirection();
		f += (n * 10);
		btVector3 from(f.x, f.y, f.z);
		btVector3 normal(n.x, n.y, n.z);

		RigidBody* rb = static_cast<RigidBody*>(EntityComponentManager::Instance()->getComponent(gameObject(), "RigidBody"));
		if (rb) rb->rigidbody()->applyCentralImpulse(normal * speed_);
	}	
}
