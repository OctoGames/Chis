#include "Transform.h"

std::string Transform::name_ = "Transform";

Transform::Transform(GameObject* container, const std::string& parent, bool enabled) : 
	Component(container, enabled)
{
	if (parent == "") node_ = OgreSystem::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("n" + gameObject()->getName());
	else node_ = OgreSystem::Instance()->getSceneManager()->getSceneNode("n" + parent)->createChildSceneNode("n" + gameObject()->getName());
}

Transform::~Transform()
{
}