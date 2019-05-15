#include "RaycastBullet.h"

#include "Enemy.h"
#include "FirstPersonCamera.h"

std::string RaycastBullet::name_ = "RaycastBullet";

RaycastBullet::RaycastBullet() :
	range_(1000.0f),
	damage_(20.0f),
	from_(btVector3(0, 0, 0)),
	normal_(btVector3(0, 0, 0)),
	hit_(RaycastHit())
{
}

RaycastBullet::~RaycastBullet()
{
}

void RaycastBullet::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_rcb"); if (it != params.end()) enabled_ = params.at("enabled_rcb").b;
	it = params.find("range"); if (it != params.end()) range_ = params.at("range").f;
	it = params.find("damage"); if (it != params.end()) damage_ = params.at("damage").f;
}

Component * RaycastBullet::clone()
{
	RaycastBullet* clonedComponent = new RaycastBullet();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->range_ = this->range_;
	clonedComponent->damage_ = this->damage_;

	return clonedComponent;
}

void RaycastBullet::init()
{
	setEnabled(enabled_);
}

void RaycastBullet::start()
{
	Ogre::Camera* cam = static_cast<FirstPersonCamera*>(EntityComponentManager::Instance()->getComponent("Player", "FirstPersonCamera"))->getCamera();
	
	if (cam)
	{
		Ogre::Vector3 from = cam->getRealPosition();
		Ogre::Vector3 normal = cam->getRealDirection();
		from += (normal * 10);
		from_ = btVector3(from.x, from.y, from.z);
		normal_ = btVector3(normal.x, normal.y, normal.z);
	}

	if (Physics::Instance()->raycast(from_, normal_, range_, &hit_))
	{
		std::cout << hit_.gameObject->getName() << "\n";

		if (hit_.gameObject->getTag() == "enemy")
		{
			Enemy* enemy = static_cast<Enemy*>(EntityComponentManager::Instance()->getComponent(hit_.gameObject, "Enemy"));
			if (enemy) enemy->damage(damage_);
		}
	}

	EntityComponentManager::Instance()->destroy(gameObject());

}
