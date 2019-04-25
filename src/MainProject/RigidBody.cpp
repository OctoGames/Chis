#include "RigidBody.h"

#include "Physics.h"

std::string RigidBody::name_ = "RigidBody";

RigidBody::RigidBody() :
	mass_(0.0),
	radius_(0.0),
	scale_(Ogre::Vector3::ZERO),
	collidedGameObjects_(std::list<GameObject*>())
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::load(const std::map<std::string, ValueType>& params)
{
	enabled_ = params.at("enabled_rb").b;
	mass_ = params.at("mass").d;
	radius_ = params.at("radius").d;
	scale_ = Ogre::Vector3(params.at("scale_rb_x").f / 2.2f, params.at("scale_rb_y").f / 2.2f, params.at("scale_rb_z").f / 2.2f);
}

Component * RigidBody::clone()
{
	RigidBody* clonedComponent = new RigidBody();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->mass_ = this->mass_;
	clonedComponent->radius_ = this->radius_;
	clonedComponent->scale_ = this->scale_;

	return clonedComponent;
}

void RigidBody::init()
{
	if (radius_ > 0.0) Physics::Instance()->createSphereRigidBody(gameObject()->transform(), mass_, radius_, "rb" + gameObject()->getGameObjectID());
	else Physics::Instance()->createBoxRigidBody(gameObject()->transform(), mass_, scale_, "rb" + gameObject()->getGameObjectID());

	collidedGameObjects_.clear();
	setEnabled(enabled_);
}

void RigidBody::fixedUpdate()
{
	clearCollidedGameObjects();
}
