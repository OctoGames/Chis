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
	auto it = params.begin();
	it = params.find("enabled_rb"); if (it != params.end()) enabled_ = params.at("enabled_rb").b;
	it = params.find("mass"); if (it != params.end()) mass_ = params.at("mass").f;
	it = params.find("radius"); if (it != params.end()) radius_ = params.at("radius").f;
	it = params.find("scale_rb_x"); if (it != params.end())
	{
		it = params.find("scale_rb_y"); if (it != params.end())
		{
			it = params.find("scale_rb_z"); if (it != params.end())
			{
				colliderHalfExtent_ = Ogre::Vector3(params.at("scale_rb_x").f, params.at("scale_rb_y").f, params.at("scale_rb_z").f);
			}
		}
	}
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
	{
		rigidBody_ = Physics::Instance()->createRigidBody(gameObject(), mass_, radius_);
	}
	// Box Shape
	else
	{
		MeshRenderer* mr = static_cast<MeshRenderer*>(EntityComponentManager::Instance()->getComponent(gameObject(), "MeshRenderer"));
		if (mr)
		{
			Ogre::Vector3 scale = gameObject()->transform()->getScale();
			Ogre::Vector3 half = mr->getEntity()->getBoundingBox().getHalfSize();
			half = Ogre::Vector3(scale.x * half.x, scale.y * half.y, scale.z * half.z);
			rigidBody_ = Physics::Instance()->createRigidBody(gameObject(), mass_, btVector3(half.x, half.y, half.z));
		}
		else
		{
			rigidBody_ = Physics::Instance()->createRigidBody(gameObject(), mass_,
				btVector3(colliderHalfExtent_.x, colliderHalfExtent_.y, colliderHalfExtent_.z));
		}
	}
	setEnabled(enabled_);
}

void RigidBody::onDestroy()
{
	Physics::Instance()->removeRigidbody(rigidBody_);
	rigidBody_ = nullptr;
}
