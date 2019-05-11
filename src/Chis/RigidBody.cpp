#include "RigidBody.h"

std::string RigidBody::name_ = "RigidBody";

RigidBody::RigidBody() :
	rigidBody_(nullptr),
	mass_(0.0f),
	radius_(0.0f),
	colliderHalfExtent_(Ogre::Vector3::ZERO)
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::load(const std::map<std::string, ValueType>& params)
{
	enabled_ = params.at("enabled_rb").b;
	mass_ = params.at("mass").f;
	radius_ = params.at("radius").f;
	colliderHalfExtent_ = Ogre::Vector3(params.at("scale_rb_x").f / 2.2f, params.at("scale_rb_y").f / 2.2f, params.at("scale_rb_z").f / 2.2f);
}

Component * RigidBody::clone()
{
	RigidBody* clonedComponent = new RigidBody();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->mass_ = this->mass_;
	clonedComponent->radius_ = this->radius_;
	clonedComponent->colliderHalfExtent_ = this->colliderHalfExtent_;

	return clonedComponent;
}

void RigidBody::init()
{
	//if (mass_ == 0) 
	//{
	//	MeshRenderer* mr = static_cast<MeshRenderer*>(EntityComponentManager::Instance()->getComponent(container_, "MeshRenderer"));
	//	rigidBody_ = Physics::Instance()->createRigidBody(gameObject(), mass_, mr->getMesh());
	//}
	//else {
	//	// Sphere Shape
	//	if (radius_ > 0.0)
	//		rigidBody_ = Physics::Instance()->createRigidBody(gameObject(), mass_, radius_);
	//	// Box Shape
	//	else
	//		rigidBody_ = Physics::Instance()->createRigidBody(gameObject(), mass_,
	//			btVector3(colliderHalfExtent_.x, colliderHalfExtent_.y, colliderHalfExtent_.z));
	//}

	// Sphere Shape
	if (radius_ > 0.0)
		rigidBody_ = Physics::Instance()->createRigidBody(gameObject(), mass_, radius_);
	// Box Shape
	else
		rigidBody_ = Physics::Instance()->createRigidBody(gameObject(), mass_,
			btVector3(colliderHalfExtent_.x, colliderHalfExtent_.y, colliderHalfExtent_.z));

	setEnabled(enabled_);
}

void RigidBody::onDisable()
{
	Physics::Instance()->removeRigidbody(rigidBody_);
	rigidBody_ = nullptr;
}

void RigidBody::onCollision(GameObject * gameObject)
{
	if (this->gameObject()->getName() != "mouse" && gameObject->getName() != "mouse") {
		std::cout << this->gameObject()->getName() << " has collided with ";
		std::cout << gameObject->getName() << std::endl;
	}
}
