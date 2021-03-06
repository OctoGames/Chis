#include "Ammo.h"

#include "AudioSource.h"
#include "RigidBody.h"
#include "GunController.h"


std::string Ammo::name_ = "Ammo";

Ammo::Ammo() :
	numBullets_(20)
{
}

Ammo::~Ammo()
{
}

void Ammo::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_a"); if (it != params.end()) enabled_ = params.at("enabled_a").b;
	it = params.find("num_bullets"); if (it != params.end()) numBullets_ = params.at("num_bullets").f;
}

Component * Ammo::clone()
{
	Ammo* clonedComponent = new Ammo();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->numBullets_ = this->numBullets_;

	return clonedComponent;
}

void Ammo::init()
{
	setEnabled(enabled_);
}

void Ammo::onCollision(GameObject * other)
{
	if (other->getTag() == "player")
	{
		GunController* gc = static_cast<GunController*>(EntityComponentManager::Instance()->getComponent("gun", "GunController"));
		if (gc) gc->addBullets(numBullets_);

		AudioSource* as = static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"));
		if (as) as->play();

		EntityComponentManager::Instance()->destroy(gameObject());
	}
	else if (other->getTag() == "ammo")
	{
		EntityComponentManager::Instance()->destroy(other);
	}
	else if (other->getTag() == "enemy")
	{
		EntityComponentManager::Instance()->destroy(gameObject());
	}
}

void Ammo::start()
{
}

void Ammo::update()
{
	float dt = RenderManager::Instance()->time()->deltaTime();
	gameObject()->transform()->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(Ogre::Degree(50)* dt));

	RigidBody* rb = static_cast<RigidBody*>(EntityComponentManager::Instance()->getComponent(gameObject(), "RigidBody"));
	rb->rigidbody()->activate();

	btTransform tr;
	Ogre::Vector3 position = gameObject()->transform()->getPosition();
	btVector3 pos(position.x, position.y, position.z);
	tr.setOrigin(pos);
	Ogre::Quaternion orientation = gameObject()->transform()->getOrientation();
	btQuaternion quat(orientation.x, orientation.y, orientation.z, orientation.w);
	tr.setRotation(quat);
	rb->rigidbody()->setCenterOfMassTransform(tr);
}
