#include "RaycastBullet.h"

#include "Enemy.h"
#include "AudioSource.h"
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

	float x, y, z;
	it = params.find("from_x"); if (it != params.end()) x = params.at("from_x").f;
	it = params.find("from_y"); if (it != params.end()) y = params.at("from_y").f;
	it = params.find("from_z"); if (it != params.end()) z = params.at("from_z").f;
	from_ = btVector3(x, y, z);

	it = params.find("normal_x"); if (it != params.end()) x = params.at("normal_x").f;
	it = params.find("normal_y"); if (it != params.end()) y = params.at("normal_y").f;
	it = params.find("normal_z"); if (it != params.end()) z = params.at("normal_z").f;
	normal_ = btVector3(x, y, z);
}

Component * RaycastBullet::clone()
{
	RaycastBullet* clonedComponent = new RaycastBullet();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->range_ = this->range_;
	clonedComponent->damage_ = this->damage_;
	clonedComponent->from_ = this->from_;
	clonedComponent->normal_ = this->normal_;


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

		AudioSource* as = static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"));
		if (as)
		{
			as->changeSource("shoot.ogg");
			as->play();
		}
	}

	if (Physics::Instance()->raycast(from_, normal_, range_, &hit_))
	{
		std::cout << hit_.gameObject->getName() << "\n";

		if (hit_.gameObject->getTag() == "enemy")
		{
			AudioSource* as = static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"));
			if (as)
			{
				as->changeSource("hit.ogg");
				as->play();
			}

			Enemy* enemy = static_cast<Enemy*>(EntityComponentManager::Instance()->getComponent(hit_.gameObject, "Enemy"));
			if (enemy) enemy->damage(damage_);
		}
	}

	EntityComponentManager::Instance()->destroy(gameObject());

}
