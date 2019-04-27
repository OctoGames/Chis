#include "RigidBody.h"

#include "Physics.h"

std::string RigidBody::name_ = "RigidBody";

RigidBody::RigidBody() :
	rigidBody_(nullptr),
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
	//enabled_ = params.at("enabled_rb").b;
	mass_ = params.at("mass").f;
	radius_ = params.at("radius").f;
	scale_ = Ogre::Vector3(params.at("scale_rb_x").f, params.at("scale_rb_y").f, params.at("scale_rb_z").f);
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
	gameObject()->transform();

	if (radius_ > 0.0) Physics::Instance()->createSphereRigidBody(gameObject()->transform(), mass_, radius_, "rb" + gameObject()->getGameObjectID());
	else Physics::Instance()->createBoxRigidBody(gameObject()->transform(), mass_, scale_, "rb" + gameObject()->getGameObjectID());
	rigidBody_ = Physics::Instance()->getRigidBodyByName("rb" + gameObject()->getGameObjectID());

	collidedGameObjects_.clear();
	setEnabled(enabled_);
}

void RigidBody::fixedUpdate()
{
	clearCollidedGameObjects();
}
