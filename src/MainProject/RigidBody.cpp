#include "RigidBody.h"
#include "Physics.h"

std::string RigidBody::name_ = "RigidBody";

RigidBody::RigidBody(GameObject* container, const std::string& parent, bool enabled) : Component(container, enabled)
{

	//We asume EVERY gameObject already has a Transform component because for it definition it has a position so it scene node has already
	//been created and we can find it by its name.
	node_ = RenderManager::Instance()->getSceneManager()->getSceneNode("n" + gameObject()->getName());
}

RigidBody::~RigidBody()
{

}

void RigidBody::createSphereRB(double mass, double radious, std::string name)
{
	Physics::Instance()->createSphereRididBody(node_, mass, radious, name);
}

void RigidBody::createBoxRB(double mass, Ogre::Vector3 scale, std::string name)
{
	Physics::Instance()->createBoxRigidBody(node_, mass, scale, name);
}